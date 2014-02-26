// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Description -----------------------------------------------------------
//
// Simple code d'exemple presentant une facon simple mais relativement lente
// de convertir un fichier .obj Wavefront vers des donnees utilisables sous
// OpenGL.
//
// A noter qu'un obj Wavefront stock les attributs de vertices en utilisant
// des indices multiples, ceci afin d'optimiser la taille du fichier final.
//
// Or OpenGL, et la plupart des API graphiques, ne supportent pas le multiple
// indexage. On ne peut en effet pas avoir des indices (elements) differents
// pour les positions et les normales par exemple.
// Il est donc necessaire de remapper les normales etc...sur les bons indices.
//

// --- Includes --------------------------------------------------------------

#include "EsgiMesh.h"
#include <cassert>
#include <cstdio>

#define ENABLE_SMOOTH_NORMALS 1

// ---

bool EsgiMesh::LoadObj(const char *path)
{
	assert(path != NULL);

	m_triangleCount = 0;

	char buffer[1024];

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		printf("[Mesh] le fichier \"%s\" ne peut être ouvert!\n", path);
		return false;
	}

#if ENABLE_SMOOTH_NORMALS
	std::vector<unsigned short> perVertexNormalCounts;
#endif
	std::vector<vec3> normals;
	std::vector<unsigned short> normal_indices;
	std::vector<vec3> tex_coords;
	std::vector<unsigned short> texcoord_indices;

	while (fgets(buffer, 1024, file) != NULL)
	{
		if (buffer[0] == 'v')
		{
			//
			// Traitement d'un vertex
			//
			if (buffer[1] == ' ')
			{
				// ----
				// todo: ajouter le support pour les vertices 2D/4D si necessaire
				// ----
				vec3 vertex;
				sscanf(&buffer[2], "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
				////vertex.z = -vertex.z;
				m_vertices.push_back(vertex);

				// HACK afin d'avoir le même nombre de normales que de vertex, c'est a dire
				// des normales aux vertices et non simplement des normales aux faces
#if ENABLE_SMOOTH_NORMALS
				perVertexNormalCounts.resize(m_vertices.size());
				for (int index = 0; index < m_vertices.size(); ++index)
				{
					perVertexNormalCounts[index] = 0;
				}
#endif
				m_normals.resize(m_vertices.size());
				m_texCoords.resize(m_vertices.size());
			}
			else if (buffer[1] == 'n')
			{
				vec3 normal;
				sscanf(&buffer[3], "%f %f %f", &normal.x, &normal.y, &normal.z);
				////normal.z = -normal.z;
				normals.push_back(normal);
			}
			else if (buffer[1] == 't')
			{
				vec3 tex;
				if (sscanf(&buffer[3], "%f %f %f", &tex.x, &tex.y, &tex.z) != 3)
				{
					if (sscanf(&buffer[3], "%f %f", &tex.x, &tex.y) != 2)
					{
						sscanf(&buffer[3], "%f", &tex.x);
					}
				}
				tex_coords.push_back(tex);
			}
		}
		else if (buffer[0] == 'f')
		{
			//
			// Traitement des indices d'une face (triangle)
			//
			unsigned short vertex_index[16], texcoord_index[16], normal_index[16];
			unsigned short num_triangles = 0;
			unsigned short num_indices = 0, num_texcoords = 0, num_normals = 0;

			// supporte une valeur de 14 chiffres au total (+ le separateur, et le NULL final)
			char valeur[16];

			char *buf = &buffer[2];
			char *substring;
			char *begin, *end;
			{		
				// Les valeurs sont separees par des espaces vides " "
				substring = strtok(buf, " ");
				while(substring != NULL)
				{
					char *end_of_substring = substring + strlen(substring);
					// extraction de l'indice du vertex
					begin = substring;
					end = strstr(begin, "/");
					int len = end - begin;
					if (len)
					{
						strncpy(valeur, begin, len);
						valeur[len] = 0;
						vertex_index[num_indices] = atoi(valeur)-1;
					}
					else {
						assert(true && "un vertex d'une face doit forcement avoir une coordonnee geometrique !");
					}
					
					// extraction de l'indice des coordonnees de texture
					begin = end + 1;					
					end = strstr(begin, "/");
					if (end == NULL) {
						end = end_of_substring;
					}
					len = end - begin;
					if (len)
					{
						strncpy(valeur, begin, len);
						valeur[len] = 0;
						texcoord_index[num_indices] = atoi(valeur)-1;
						num_texcoords++;
					}

					if (end != end_of_substring)
					{
						// extraction de l'indice des normales
						begin = end + 1;					
						end = end_of_substring;
						len = end - begin;
						if (len)
						{
							strncpy(valeur, begin, len);
							valeur[len] = 0;
							normal_index[num_indices] = atoi(valeur)-1;
	#if ENABLE_SMOOTH_NORMALS
							// Pour chaque vertex on effectue une somme des normales aux faces
							// desquelles il fait parti de la topologie.
							if (perVertexNormalCounts[vertex_index[num_indices]] == 0) {
								m_normals[vertex_index[num_indices]] = vec3(0.f, 0.f, 0.f);
							}
							perVertexNormalCounts[vertex_index[num_indices]]++;
							m_normals[vertex_index[num_indices]] += normals[normal_index[num_indices]];
	#else
							// force une seule normale par vertex
							// celle stockee en dernier sera la normale du vertex
							m_normals[vertex_index[num_indices]] = normals[normal_index[num_indices]];
	#endif
							num_normals++;
						}
					}

					num_indices++;
					assert(num_indices <= 16);

					substring = strtok(NULL, " ");
				}

				assert(num_indices >= 3);
				num_triangles = num_indices-2;
				m_triangleCount += num_triangles;

				unsigned short index = 1;
				for (unsigned short tri = 0; tri < num_triangles; ++tri)
				{
					m_indices.push_back(vertex_index[0]);
					m_indices.push_back(vertex_index[index]);
					m_indices.push_back(vertex_index[index+1]);
					/*if (num_texcoords)
					{
						texcoord_indices.push_back(texcoord_index[0]);
						texcoord_indices.push_back(texcoord_index[index+1]);
						texcoord_indices.push_back(texcoord_index[index+2]);
					}
					if (num_normals)
					{
						normal_indices.push_back(normal_index[0]);
						normal_indices.push_back(normal_index[index+1]);
						normal_indices.push_back(normal_index[index+2]);
					}*/

					++index;
				}				
			}
		}
		else if (buffer[0] == 'g')
		{
			m_name = &buffer[2];
		}
	}

	fclose(file);

#if ENABLE_SMOOTH_NORMALS
	// post process des normales: 
	// normale du vertex = une simple moyenne des normales des faces partagees par le vertex
	//
	for (int index = 0; index < m_vertices.size(); ++index)
	{
		m_normals[index] /= (float)perVertexNormalCounts[index];
	}
#endif

	return true;
}