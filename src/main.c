#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

/* OpenGL */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Nuklear */
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#include "../lib/nuklear.h"
#include "../lib/nuklear_glfw_gl3.h"

/* Headers of project */
#include "gui.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

void
error_callback(int error, const char* description)
{
	fprintf(stderr, "[GLFW] Error: \n %d: %s\n", error, description);
}

int
main(void)
{
	int                       folder_selected;
	/* GLFW */
	struct nk_glfw            glfw = {0};
	static GLFWwindow        *win;
	int                       width;
	int                       height;

	/* GUI */
	struct nk_context        *ctx;
	struct nk_font_atlas     *atlas;

	/* OpenGl init */
	width = 0;
	height = 0;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		fprintf(stderr, "[GLFW] Failed to init!\n");
		exit(1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* create window */
	win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "fpsR", NULL, NULL);
	glfwMakeContextCurrent(win);
	glfwGetWindowSize(win, &width, &height);
	glewExperimental = 1;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "[GLEW] Failed to setup!\n");
		exit(1);
	}
	/* nuklear context and font */
	ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
	nk_glfw3_font_stash_begin(&glfw, &atlas);
	struct nk_font *dejavu = nk_font_atlas_add_from_file(atlas, "assets/DejaVuSansMonoNerdFontComplete.ttf", 14, 0);
	nk_glfw3_font_stash_end(&glfw);
	nk_style_set_font(ctx, &dejavu->handle);
	init_icons();

	folder_selected = FOLDER_NOT_SELECTED;
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
		nk_glfw3_new_frame(&glfw);

		if (folder_selected == FOLDER_NOT_SELECTED) {
			folder_selected = init_folder_selector(ctx, width, height);
		} else {
			init_fps(ctx, width, height);
		}

		/* Redraw buffer */
		glfwGetWindowSize(win, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
		glfwSwapBuffers(win);
	}
}
