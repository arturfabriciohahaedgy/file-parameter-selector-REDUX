#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <string.h>

#include "../lib/nuklear.h"
#include "../lib/tinyfiledialogs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

#include "gui.h"

#define BUFFER_LENGHT 256

struct images {
	struct nk_image open_folder;
	struct nk_image confirm;
};
struct images icons;


static char field_buffer[BUFFER_LENGHT];

static struct nk_image
load_image(char *filepath)
{
	int x, y, n;
	GLuint tex;
	unsigned char *data = stbi_load(filepath, &x, &y, &n, 0);
	if (!data) {
		fprintf(stderr, "Failed to load image: %s", filepath);
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
init_icons(void)
{
	icons.open_folder = load_image("assets/open_folder.png");
	icons.confirm = load_image("assets/confirm.png");
}

void
init_fps(struct nk_context *ctx, int ww, int wh)
{
	if (nk_begin(ctx, "fpsR", nk_rect(0, 0, ww, wh), NK_WINDOW_BORDER)) {
		nk_layout_row_dynamic(ctx, 120, 1);
		nk_label(ctx, "Hello world!", NK_TEXT_CENTERED);
	}
	nk_end(ctx);
}

int
init_folder_selector(struct nk_context *ctx, int ww, int wh)
{
	static char *folder_path;
	static int  path_len;
	static int  field_len;

	if (nk_begin(ctx, "Input folder path: ", nk_rect(0, 0, ww, wh), NK_WINDOW_BORDER|NK_WINDOW_TITLE)) {
		/* row for nk_edit_string and the button with the folder icon */
		nk_layout_row_static(ctx, (wh / 3.0), 50, 1);
		/* starts to place the text box and the "open folder" button on the window */
		nk_layout_row_begin(ctx, NK_STATIC, 50, 4);
		nk_layout_row_push(ctx, (ww / 3.0));
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 170);
		nk_edit_string(ctx, NK_EDIT_FIELD, field_buffer, &field_len, BUFFER_LENGHT, nk_filter_default);
		nk_layout_row_push(ctx, 30);
		nk_spacing(ctx, 1);
		nk_layout_row_push(ctx, 50);
		if (nk_button_image(ctx, icons.open_folder)) {
		    folder_path = tinyfd_selectFolderDialog("Select a folder", ".");
		    path_len = strlen(folder_path);
		    memcpy(field_buffer, folder_path, path_len);
		    field_len += path_len;
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
			return FOLDER_SELECTED;
		}
		nk_layout_row_end(ctx);
	}
	printf("field_buffer: %s\n", field_buffer);
	printf("field_len: %d\n", field_len);
	nk_end(ctx);
	return FOLDER_NOT_SELECTED;
}
