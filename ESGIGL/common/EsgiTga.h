// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_TGA
#define ESGI_TGA

// --- Includes --------------------------------------------------------------

#include "../Plateforme/EsgiPlateforme.h"
#include <cstdio>

// esgiTexture ---

struct EsgiTexture
{
    GLuint width; 
    GLuint height;
    GLenum datatype;
    GLenum format;
    GLint  internalFormat;
    GLuint id;

    GLubyte *texels;
};

// EsgiTgaHeader ---
// librement inspire de http://tfc.duke.free.fr/coding/tga.html
// Le taille de la structure doit être de 18 octets
// Il faut donc modifier l'alignement de base pour cette structure a l'octet pres

#pragma pack(push, 1)
struct EsgiTgaHeader
{
  GLubyte id_length;          /* size of image id */
  GLubyte colormap_type;      /* 1 is has a colormap */
  GLubyte image_type;         /* compression type */
  // sans le pragma pack a 1, un octet de bourrage aurait ete insere ici!
  short	cm_first_entry;       /* colormap origin */
  short	cm_length;            /* colormap length */
  GLubyte cm_size;            /* colormap size */
  // sans le pragma pack a 1, un octet de bourrage aurait ete insere ici!
  short	x_origin;             /* bottom left x coord origin */
  short	y_origin;             /* bottom left y coord origin */

  short	width;                /* picture width (in pixels) */
  short	height;               /* picture height (in pixels) */

  GLubyte pixel_depth;        /* bits per pixel: 8, 16, 24 or 32 */
  GLubyte image_descriptor;   /* 24 bits = 0x00; 32 bits = 0x80 */
};
#pragma pack(pop)

void GetTextureInfo(const EsgiTgaHeader *header, EsgiTexture *texinfo)
{
	texinfo->width = header->width;
	texinfo->height = header->height;

	switch (header->image_type)
	{
		case 3:  /* Grayscale 8 bits */
		case 11: /* Grayscale 8 bits (RLE) */
		{
			if (header->pixel_depth == 8)
			{
                texinfo->datatype = GL_UNSIGNED_BYTE;
				texinfo->format = GL_LUMINANCE;
				texinfo->internalFormat = 1;
			}
			else /* 16 bits */
			{
                texinfo->datatype = GL_UNSIGNED_BYTE;
                texinfo->format = GL_LUMINANCE_ALPHA;
                texinfo->internalFormat = 2;
			}
		break;
		}

		case 1:  /* 8 bits color index */
		case 2:  /* BGR 16-24-32 bits */
		case 9:  /* 8 bits color index (RLE) */
		case 10: /* BGR 16-24-32 bits (RLE) */
		{
			/* 8 bits and 16 bits images will be converted to 24 bits */
			if (header->pixel_depth <= 24)
			{
                texinfo->datatype = GL_UNSIGNED_BYTE;
				texinfo->format = GL_RGB;
				texinfo->internalFormat = 3;
			}
			else /* 32 bits */
			{
                texinfo->datatype = GL_UNSIGNED_BYTE;
				texinfo->format = GL_RGBA;
				texinfo->internalFormat = 4;
			}

			break;
		}
	}
}

void ReadTGA8bits(FILE *fp, const GLubyte *colormap, EsgiTexture *texinfo)
{
	GLubyte color;

	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
		/* Read index color byte */
		color = (GLubyte)fgetc(fp);

		/* Convert to RGB 24 bits */
		texinfo->texels[(i * 3) + 2] = colormap[(color * 3) + 0];
		texinfo->texels[(i * 3) + 1] = colormap[(color * 3) + 1];
		texinfo->texels[(i * 3) + 0] = colormap[(color * 3) + 2];
	}
}

void ReadTGAgray8bits(FILE *fp, EsgiTexture *texinfo)
{
	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
		/* Read grayscale color byte */
		texinfo->texels[i] = (GLubyte)fgetc(fp);
	}
	/* Pour ce type d'image, on peut même lire les données en une instruction */
	//fread(texinfo->texels, sizeof (GLubyte), texinfo->width * texinfo->height, fp);
}

void ReadTGA16bits(FILE *fp, EsgiTexture *texinfo)
{
	GLushort color;

	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
		/* Read color word */
		color = fgetc (fp) + (fgetc(fp) << 8);

		/* Convert BGR to RGB */
		texinfo->texels[(i * 3) + 0] = (GLubyte)(((color & 0x7C00) >> 10) << 3);
		texinfo->texels[(i * 3) + 1] = (GLubyte)(((color & 0x03E0) >>  5) << 3);
		texinfo->texels[(i * 3) + 2] = (GLubyte)(((color & 0x001F) >>  0) << 3);
	}
}

void ReadTGAgray16bits(FILE *fp, EsgiTexture *texinfo)
{
	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
      /* Read grayscale color + alpha channel bytes */
      texinfo->texels[(i * 2) + 0] = (GLubyte)fgetc(fp);
      texinfo->texels[(i * 2) + 1] = (GLubyte)fgetc(fp);
	}
}

void ReadTGA24bits(FILE *fp, EsgiTexture *texinfo)
{
	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
		/* Read and convert BGR to RGB */
		texinfo->texels[(i * 3) + 2] = (GLubyte)fgetc(fp);
		texinfo->texels[(i * 3) + 1] = (GLubyte)fgetc(fp);
		texinfo->texels[(i * 3) + 0] = (GLubyte)fgetc(fp);
	}
}

void ReadTGA24bitsRLE (FILE *fp, EsgiTexture *texinfo)
{
	int i, size;
	GLubyte rgb[3];
	GLubyte packet_header;
	GLubyte *ptr = texinfo->texels;

	while (ptr < texinfo->texels + (texinfo->width * texinfo->height) * 3)
	{
		/* Read first byte */
		packet_header = (GLubyte)fgetc (fp);
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			/* Run-length packet */
			fread (rgb, sizeof (GLubyte), 3, fp);

			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[0] = rgb[2];
				ptr[1] = rgb[1];
				ptr[2] = rgb[0];
			}
		}
		else
		{
			/* Non run-length packet */
			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[2] = (GLubyte)fgetc(fp);
				ptr[1] = (GLubyte)fgetc(fp);
				ptr[0] = (GLubyte)fgetc(fp);
			}
		}
	}
}

void ReadTGA32bits(FILE *fp, EsgiTexture *texinfo)
{
	for (int i = 0; i < texinfo->width * texinfo->height; ++i)
	{
		/* Read and convert BGRA to RGBA */
		texinfo->texels[(i * 4) + 2] = (GLubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 1] = (GLubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 0] = (GLubyte)fgetc(fp);
		texinfo->texels[(i * 4) + 3] = (GLubyte)fgetc(fp);
	}
}

struct EsgiTexture* esgiReadTGAFile(const char *filename)
{
	FILE *fp;
	EsgiTexture *texinfo;
	EsgiTgaHeader header;
	GLubyte *colormap = NULL;

	fp = fopen(filename, "rb");
	if (!fp)
	{
		fprintf(stderr, "error: couldn't open \"%s\"!\n", filename);
		return NULL;
	}

	/* Lecture de l'en-tête du fichier */
	fread(&header, sizeof (EsgiTgaHeader), 1, fp);

	texinfo = new EsgiTexture;
	GetTextureInfo(&header, texinfo);
	fseek(fp, header.id_length, SEEK_CUR);

	/* Lecture de la palette... */
	if (header.colormap_type)
	{
		/* NOTE: color map is stored in BGR format */
		colormap = new GLubyte[header.cm_length * (header.cm_size >> 3)];
		if (!colormap)
		{
			delete texinfo;
			return NULL;
		}
		fread (colormap, sizeof (GLubyte), header.cm_length * (header.cm_size >> 3), fp);
	}

	/* Allocations mémoire nécessaires... */
	texinfo->texels = new GLubyte[texinfo->width * texinfo->height * texinfo->internalFormat];
	if (!texinfo->texels)
	{
		delete[] colormap;
		delete texinfo;
		return NULL;
	}

	/* Lecture des données pixels de l'image... */
	switch (header.image_type)
	{
		case 0:
		/* No data */
		break;

		case 1:
		/* Uncompressed 8 bits color index */
		ReadTGA8bits(fp, colormap, texinfo);
		break;

		case 2:
		/* Uncompressed 16-24-32 bits */
		switch (header.pixel_depth)
		{
			case 16:
			ReadTGA16bits(fp, texinfo);
			break;
			case 24:
			ReadTGA24bits(fp, texinfo);
			break;
			case 32:
			ReadTGA32bits(fp, texinfo);
			break;
		}
		break;

		case 3:
		/* Uncompressed 8 or 16 bits grayscale */
		if (header.pixel_depth == 8)
			ReadTGAgray8bits(fp, texinfo);
		else /* 16 */
			ReadTGAgray16bits(fp, texinfo);

		break;

		case 9:
		/* RLE compressed 8 bits color index */
		//ReadTGA8bitsRLE(fp, colormap, texinfo);
		break;

		case 10:
		/* RLE compressed 16-24-32 bits */
		switch (header.pixel_depth)
		{
			case 16:
			//ReadTGA16bitsRLE(fp, texinfo);
			break;
			case 24:
			ReadTGA24bitsRLE(fp, texinfo);
			break;
			case 32:
			//ReadTGA32bitsRLE(fp, texinfo);
			break;
		}
		break;

		case 11:
		/* RLE compressed 8 or 16 bits grayscale */
		//if (header.pixel_depth == 8)
		//ReadTGAgray8bitsRLE(fp, texinfo);
		//else /* 16 */
		//ReadTGAgray16bitsRLE(fp, texinfo);
		break;

		default:
		/* Image type is not correct */
		fprintf (stderr, "error: unknown TGA image type %i!\n", header.image_type);
		delete[] texinfo->texels;
		delete texinfo;
		texinfo = NULL;
		break;
	}
	
	/* Désallocations mémoire nécessaires... */
	if (colormap) {
		delete[] colormap;
	}

	fclose(fp);
	return texinfo;
}

#endif // ESGI_TGA