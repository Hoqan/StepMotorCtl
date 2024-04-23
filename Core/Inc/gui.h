#ifndef _GUI_H
#define _GUI_H

typedef enum
{
	NORMAL,
	SETTINGS
} GuiState;

extern GuiState guiState;


void guiTask(void);

#endif /* _GUI_H */
