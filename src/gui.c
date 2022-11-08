#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glew.h>

#define NK_INCLUDE_STANDARD_VARARGS
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
	struct nk_image add;
};
struct images icons;

/* popups */
nk_bool insertcommand = nk_true;
nk_bool commandinserted = nk_false;

/* commands */
char maincommand[BUFFER_LENGHT];

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
	icons.add =        loadimage("assets/add.png");
}

void
/* errorpopup(char *message) */
errorpopup(struct nk_context *ctx)
{
	int w = 300, h = 300;
	struct nk_rect a = {0, 0, w, h};
	if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Error!", NK_WINDOW_CLOSABLE|NK_WINDOW_SCALABLE|NK_WINDOW_MOVABLE, a)) {
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
	static int     fieldlen;
	static char    fieldbuffer[BUFFER_LENGHT];
	struct nk_rect s = {100, 100, 250, 250};

	/* width-half */
	float whalf = ((ww - 25) / 2.0);
	if (nk_begin(ctx, "fpsR", nk_rect(0, 0, ww, wh), NK_WINDOW_TITLE|NK_WINDOW_BORDER)) {
		nk_layout_row_begin(ctx, NK_STATIC, 0, 3);
		nk_layout_row_push(ctx, whalf);
		nk_label(ctx, "AAAAAA", NK_TEXT_CENTERED);
		nk_layout_row_push(ctx, (whalf - 250));
		if (commandinserted) {
			fieldlen = 0;
			memset(fieldbuffer, 0, strlen(fieldbuffer));
			nk_labelf(ctx, NK_TEXT_LEFT, "Command: %s", maincommand);
		}
		else
			nk_label(ctx, "Command: ", NK_TEXT_LEFT);
		nk_layout_row_push(ctx, ww);
		if (nk_button_text(ctx, "Change command", 14)) {
			commandinserted = nk_false;
			insertcommand = nk_true;
		}
		nk_layout_row_end(ctx);
		if (insertcommand) {
			if (nk_popup_begin(ctx, NK_POPUP_STATIC, "Command input", NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE, s)) {
				nk_layout_row_dynamic(ctx, 25, 1);
				nk_label(ctx, "Insert a command: ", NK_TEXT_LEFT);
				nk_layout_row_dynamic(ctx, 25, 1);
				nk_edit_string(ctx, NK_EDIT_FIELD, fieldbuffer, &fieldlen, BUFFER_LENGHT, nk_filter_default);
				nk_layout_row_static(ctx, 50, 0, 2);
				nk_layout_row_begin(ctx, NK_STATIC, 50, 2);
				nk_layout_row_push(ctx, ((250/2) - 35));
				nk_spacing(ctx, 1);
				nk_layout_row_push(ctx, 50);
				if (nk_button_image(ctx, icons.confirm)) {
					strcpy(maincommand, fieldbuffer);
					nk_popup_close(ctx);
					insertcommand = nk_false;
					commandinserted = nk_true;
				}
				nk_layout_row_end(ctx);
				nk_popup_end(ctx);
			}  else
				insertcommand = nk_false;
		}
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
		nk_layout_row_static(ctx, 10, 0, 1);
		/* row for the "confirm" button icon */
		/* nk_layout_row_begin(ctx, NK_STATIC, 50, 2);
		nk_layout_row_push(ctx, (ww / 2.0));
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 50); */
		nk_layout_row_static(ctx, 10, ((ww - 25) / 3), 3);
		if (nk_button_image(ctx, icons.confirm)) {
			nk_end(ctx);
			returndirs(fieldbuffer, 0, &arg);
			return FOLDER_SELECTED;
		}
		/* nk_layout_row_end(ctx); */
	}
	nk_end(ctx);
	return FOLDER_NOT_SELECTED;
}
