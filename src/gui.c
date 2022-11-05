#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glew.h>

#include "../lib/nuklear.h"
#include "../lib/tinyfiledialogs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "gui.h"
#include "files.h"

#define BUFFER_LENGHT 256

struct images {
	struct nk_image openfolder;
	struct nk_image confirm;
	struct nk_image error;
};
struct images icons;

static struct nk_image
loadimage(char *filepath)
{
	int x, y, n;
	GLuint tex;
	unsigned char *data = stbi_load(filepath, &x, &y, &n, 0);
	if (!data) {
		fprintf(stderr, "Failed to load image: %s\n", filepath);
		exit(1);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return nk_image_id((int)tex);
}

void
initicons(void)
{
	icons.openfolder = loadimage("assets/open_folder.png");
	icons.confirm =    loadimage("assets/confirm.png");
	icons.error =      loadimage("assets/error.png");
}

void
/* errorpopup(char *message) */
errorpopup(struct nk_context *ctx)
{
	int w = 300, h = 300;
	struct nk_rect s = {0, 0, w, h};
	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error!", NK_WINDOW_CLOSABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MOVABLE, s)) {
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		nk_layout_row_push(ctx, 50);
		nk_image(ctx, icons.error);
		nk_layout_row_end(ctx);
		nk_popup_end(ctx);
	}
}

void
initfps(struct nk_context *ctx, int ww, int wh)
{
	if (nk_begin(ctx, "fpsR", nk_rect(0, 0, ww, wh), NK_WINDOW_BORDER)) {
		/* nk_layout_row_static(ctx, (wh - 25), (ww -25), 1); */
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		nk_layout_row_push(ctx, ((ww - 27) / 2.0));
		nk_label(ctx, "aaaa", NK_TEXT_CENTERED);
		nk_layout_row_push(ctx, ((ww - 27) / 2.0));
		nk_label(ctx, "Command: ", NK_TEXT_LEFT);
		if (nk_button_label(ctx, "Click here!"))
			errorpopup(ctx);
		nk_layout_row_end(ctx);
	}
	nk_end(ctx);
}

int
initfolderselector(struct nk_context *ctx, int ww, int wh)
{
	static char *folderpath;
	static int   pathlen;
	static int   fieldlen;
	static char  fieldbuffer[BUFFER_LENGHT];

	if (nk_begin(ctx, "Input folder path: ", nk_rect(0, 0, ww, wh), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
		/* row for nk_edit_string and the button with the folder icon */
		nk_layout_row_static(ctx, (wh / 3.0), 50, 1);
		/* starts to place the text box and the "open folder" button on the window */
		nk_layout_row_begin(ctx, NK_STATIC, 50, 4);
		nk_layout_row_push(ctx, (ww / 3.0));
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 170);
		nk_edit_string(ctx, NK_EDIT_SIMPLE, fieldbuffer, &fieldlen, BUFFER_LENGHT, nk_filter_default);
		nk_layout_row_push(ctx, 30);
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 50);
		if (nk_button_image(ctx, icons.openfolder)) {
		    folderpath = tinyfd_selectFolderDialog("Select a folder", ".");
		    if (folderpath) {
				pathlen = strlen(folderpath);
				memcpy(fieldbuffer, folderpath, pathlen);
				fieldlen += pathlen;
		    }
		}
		nk_layout_row_end(ctx);
		/* spacing */
		nk_layout_row_static(ctx, 10, 50, 1);
		/* row for the "confirm" button icon */
		nk_layout_row_begin(ctx, NK_STATIC, 50, 2);
		nk_layout_row_push(ctx, (ww / 2.0));
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 50);
		if (nk_button_image(ctx, icons.confirm)) {
			nk_end(ctx);
			returndirs(fieldbuffer, 0, &arg);
			return FOLDER_SELECTED;
		}
		nk_layout_row_end(ctx);
	}
	nk_end(ctx);
	return FOLDER_NOT_SELECTED;
}
