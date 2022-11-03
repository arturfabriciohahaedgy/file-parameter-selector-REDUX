#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FOLDER_SELECTED 0
#define FOLDER_NOT_SELECTED 1

void initicons(void);

int initfolderselector(struct nk_context*, int, int);
void initfps(struct nk_context*, int, int);

