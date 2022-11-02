#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOLDER_SELECTED 0
#define FOLDER_NOT_SELECTED 1

void init_icons(void);

int init_folder_selector(struct nk_context*, int, int);
void init_fps(struct nk_context*, int, int);

