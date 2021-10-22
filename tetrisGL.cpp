/*************************************
*                                    *
*      Jeff Molofee's Lessons        *
*          nehe.gamedev.net          *
*                2002                *
*                                    *
*************************************/

#include <windows.h>						// Header File For Windows
#include <math.h>
#include <stdio.h>							// Header File For Standard Input/Output
#include <errno.h>
#include <gl\gl.h>							// Header File For The OpenGL32 Library
#include <gl\glu.h>							// Header File For The GLu32 Library
#include "bmp.h"						// Header File For The GLaux Library
#include "NeHeGL.h"							// Header File For NeHeGL
#include "resource.h"						// Header File For Resource (*IMPORTANT*)
#include "pieces.h"							// Header File For Tetris Pieces
#include "board.h"							// Header File For Tetris Board




#pragma comment( lib, "opengl32.lib" )		// Search For OpenGL32.lib While Linking
#pragma comment( lib, "glu32.lib" )			// Search For GLu32.lib While Linking
// #pragma comment( lib, "glaux.lib" )			// Search For GLaux.lib While Linking

#ifndef CDS_FULLSCREEN						// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4					// Compilers. By Defining It This Way,
#endif										// We Can Avoid Errors


GL_Window*	g_window;
Keys*		g_keys;
MouseCoords* g_mouseCoord;


BOOL	light;									// Lighting ON / OFF
GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // Ambient Light Values ( NEW )
//GLfloat LightDiffuse[]= { 0.8f, 0.8f, 0.4f, 1.0f };	// Diffuse Light Values ( NEW ) -> coming from a ligth source
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 0.0f, 1.0f };	// Diffuse Light Values ( NEW ) -> coming from a ligth source
//left, right, depth into screen, This tells OpenGL the designated coordinates are the position of the light source
GLfloat LightPosition[]= { 0.0f, 0.0f, 10.0f, 2.0f };				 // Light Position ( NEW )

bool fp, lp, bp, blend, upp, key_space_pressed;

float xpos = 0.0f;
float zpos = 1.0f; 
float ypos = 0.0f;

float xrot = 0;

GLuint	filter;				// Which Filter To Use
GLuint	texture[4];			// Storage For 3 Textures

float g_PieceWidth = 0.02f;  //piece width
float g_PieceHeight = 0.02f; //piece height
float g_PieceDepth = 0.02f; //piece depth

//declarations for tetris
pieces MyPieces; //pieces object
board MyBoard; //board object
int pieceNotMoved; //variable which indicates if the piece was moved
int updateCycle; // the current cycle to update the piece movement
bool gameOver = true; // semaphor for game over
bool gamePaused = false; // semaphor for game paused

GLuint	base;							// Base Display List For The Font Set
GLfloat	rot;							// 1st Counter Used To Move Text & For Coloring
GLYPHMETRICSFLOAT gmf[256];					// Storage For Information About Our Font


//forwarding declarations
BOOL InitGL();
BOOL DrawGLScene();
int LoadGLTextures();
void BuildFont();
GLvoid glPrint(GLint x, GLint y, int set, char *string, ... );
GLvoid KillFont(GLvoid);									// Delete The Font From Memory
BOOL CreateNewPiece();

//initialisation for first piece and board
BOOL LocalInit()
{
	pieceNotMoved = 0;
	updateCycle = 0;
	MyBoard.InitialiseBoard();
	MyBoard._x = -0.1f; //board position
	MyBoard._y = 0.2f;

	CreateNewPiece();

	return TRUE;
}

//creates the first piece
BOOL CreateNewPiece()
{
	MyPieces.CreateNewPiece();
	MyPieces.currentPiece->_x = rand() % BoardColumns - MyPieces.currentPiece->_width;
	if (MyPieces.currentPiece->_x <0 )
		MyPieces.currentPiece->_x =0;

	return TRUE;

}

// initialisations for control status
BOOL Initialize (GL_Window* window, Keys* keys, MouseCoords* mouseCoord)					// Any GL Init Code & User Initialiazation Goes Here
{
	g_window	= window;
	g_keys		= keys;
	g_mouseCoord = mouseCoord;
	LocalInit();

	return InitGL();
}


//set up the OpenGL
BOOL InitGL()								// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}

	BuildFont();						// Build The Font

	
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
   
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light   
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light

	glEnable(GL_LIGHT1);							// Enable Light One

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glDepthFunc(GL_LEQUAL);					// The Type Of Depth Testing To Do	
	
	//glEnable(GL_LIGHTING);					// Enable Lighting				( NEW )	


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Test To Do
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Select The Type Of Blending
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glEnable(GL_TEXTURE_2D);							// Enable 2D Texture Mapping
	return TRUE;	
}


//free memory
void Deinitialize (void)										// Any User DeInitialization Goes Here
{
	KillFont();
}



//function for returning a file pointer file
AUX_RGBImageRec *LoadBMP(char *Filename)					// Loads A Bitmap Image
{
	FILE *File=NULL;							// File Handle
    
	if (!Filename)								// Make Sure A Filename Was Given
	{
		return NULL;							// If Not Return NULL
	}

	errno_t err;
     
	err = fopen_s(&File, Filename,"r");						// Check To See If The File Exists
    
	if (err == 0 && File)								// Does The File Exist?
	{
		fclose(File);							// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}
      
	return NULL;								// If Load Failed Return NULL
}

//draw a cube at the given position
BOOL DrawGLQuad(float xTopLeft, float yTopLeft, float width, float height, float depth)
{
	float x = -2.0f;
	glBegin(GL_QUADS);							// Start Drawing Quads	

	/*glTexCoord2f(0.0f, 0.0f); glVertex3f(xTopLeft,  yTopLeft - height,  depth);	// Bottom left (Front)	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xTopLeft + width,  yTopLeft - height,  depth);	// Bottom Right (Front)	
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xTopLeft + width, yTopLeft,  depth);	// Top right(Front)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xTopLeft, yTopLeft, depth);	// Top Left (Front)
	*/
	// Bottom Face
/*	glTexCoord2f(1.0f, 1.0f); glVertex3f( xTopLeft + width,  yTopLeft,  x);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( xTopLeft,  yTopLeft,  x);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( xTopLeft, yTopLeft - height,  x);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( xTopLeft + width, yTopLeft - height,  x);	// Bottom Right Of The Texture and Quad
	
	*/	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( xTopLeft, yTopLeft - height,  0);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( xTopLeft + width, yTopLeft - height,  0);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( xTopLeft + width,  yTopLeft,  0);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( xTopLeft,  yTopLeft,  0);	// Top Left Of The Texture and Quad
	// Back Face
	glNormal3f( 0.0f, 0.0f, -2.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( xTopLeft + width, yTopLeft - height, -depth);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( xTopLeft + width, yTopLeft,  -depth);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( xTopLeft,  yTopLeft,  -depth);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( xTopLeft, yTopLeft - height,  -depth);	// Bottom Left Of The Texture and Quad
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( xTopLeft + width, yTopLeft - height, - depth);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( xTopLeft + width,  yTopLeft, - depth);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( xTopLeft + width,  yTopLeft,  0);	// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( xTopLeft + width, yTopLeft - height,  0);	// Bottom Left Of The Texture and Quad
	// Left Face
	glNormal3f( -1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(xTopLeft, yTopLeft - height, -depth);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(xTopLeft, yTopLeft - height,  0);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(xTopLeft, yTopLeft,  0);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(xTopLeft, yTopLeft, -depth);	// Top Left Of The Texture and Quad


	glEnd();

	return TRUE;
}

//draw a tetris piece, according to it`s position
BOOL DrawPiece(piece * pLocalPiece, bool nextPiece)
{
	int matrix[PieceMaxRows][PieceMaxRows];

	pLocalPiece->GetPieceMatrix(matrix);

	for (int rowIndex = 0; rowIndex< PieceMaxRows; rowIndex ++)
		for (int colIndex = 0; colIndex< PieceMaxRows; colIndex ++)
		{

			if (matrix[rowIndex][colIndex] == 1)
			{
				if (nextPiece)
				DrawGLQuad(colIndex*g_PieceWidth + MyBoard._x + (BoardColumns + 3)* g_PieceWidth, 
							- g_PieceHeight * rowIndex + MyBoard._y,
					g_PieceWidth, g_PieceHeight, g_PieceDepth);
				else
				DrawGLQuad((pLocalPiece->_x  + colIndex)*g_PieceWidth + MyBoard._x, 
							- g_PieceHeight * (pLocalPiece->_y + rowIndex) + MyBoard._y,
					g_PieceWidth, g_PieceHeight, g_PieceDepth);
			}
		}
	
	return TRUE;
}

//draws the border
BOOL DrawBackground()
{
	DrawGLQuad(MyBoard._x - g_PieceWidth, MyBoard._y, g_PieceWidth, g_PieceHeight * BoardRows, g_PieceDepth); //draw left margin
	DrawGLQuad(MyBoard._x + BoardColumns * g_PieceWidth, MyBoard._y, g_PieceWidth, g_PieceHeight * BoardRows,g_PieceDepth); //draw right margin
	//DrawGLQuad(MyBoard._x - g_PieceWidth, MyBoard._y - g_PieceHeight, g_PieceWidth*(BoardColumns+1), g_PieceHeight, g_PieceDepth); //draw top margin
	DrawGLQuad(MyBoard._x - g_PieceWidth, MyBoard._y - g_PieceHeight*BoardRows, g_PieceWidth*(BoardColumns+2), g_PieceHeight, g_PieceDepth); //draw bottom margin

	return TRUE;
}

//draws the board
BOOL DrawBoard()
{

	int matrix[BoardRows][BoardColumns];
	MyBoard.GetBoardMatrix(matrix);

	for (int rowIndex = 0; rowIndex< BoardRows; rowIndex ++)
		for (int colIndex = 0; colIndex< BoardColumns; colIndex ++)
		{

			if (matrix[rowIndex][colIndex] == 1)
			{
				DrawGLQuad(colIndex*g_PieceWidth + MyBoard._x, 
							- g_PieceHeight * rowIndex + MyBoard._y,
					g_PieceWidth, g_PieceHeight, g_PieceDepth);				
				
			}			
		}
	return TRUE;
}


// draws the scene
BOOL DrawGLScene()								// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View

	glColor3f(1.0f,1.0f,1.0f); //set the color to white
	GLfloat xtrans = xpos; //the camera x position
	GLfloat ztrans = -zpos; //the camera z position
	GLfloat ytrans = ypos; //the camera y position


	glTranslatef(xtrans, ytrans, ztrans); //set the camera
	glRotatef(xrot, 0.0f, 1.0f, 0.0f); //set the rotation on x
	glBindTexture(GL_TEXTURE_2D, texture[filter]); //set the current filter

	glDisable(GL_BLEND);								// Disable Blending Before We Draw In 3D
	if (!gameOver)
	{
		//draws the current and next pieces
		DrawPiece(MyPieces.currentPiece, false);
		DrawPiece(MyPieces.nextPiece, true);
	}

	DrawBoard();
	DrawBackground();
	
	glEnable(GL_BLEND);								// Enable Blending

	if (gameOver)
	{
		glPrint(550 ,150, 0, "Press Enter to start game");
		glPrint(570 ,175, 0, "Game Over");
	}

	
	rot+=0.5f;						// Increase The Rotation Variable	
	
	glLoadIdentity();									// Reset The View
	glColor3f(1.0f,1.0f,1.0f);							// Set Color To White
	glPrint(550 ,350, 0, "Score: %i",MyBoard.score);
	glPrint(550 ,300, 0, "Level: %i",MyBoard.level);
	glPrint(550 ,275, 0, "Next Level Score: %i",MyBoard.score_level[MyBoard.level - 1]);
	glPrint(550 ,250, 0, "Drop Speed: %i",MyBoard.dropSpeed);
	

	return TRUE;										// Everything Went OK
}


int LoadGLTextures()								// Load Bitmaps And Convert To Textures
{
        int Status=FALSE;                               // Status Indicator

        AUX_RGBImageRec *TextureImage[2];               // Create Storage Space For The Texture

        memset(TextureImage,0,sizeof(void *)*2);        // Set The Pointer To NULL

        // Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit        
		if ( (TextureImage[0]=LoadBMP("Data/Crate.bmp")) && (TextureImage[1]=LoadBMP("Data/Font.bmp")) )
        {
                Status=TRUE;                            // Set The Status To TRUE

                glGenTextures(4, &texture[0]);          // Create Three Textures

				// Create Nearest Filtered Texture
				glBindTexture(GL_TEXTURE_2D, texture[0]);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

                // Create Linear Filtered Texture
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

				// Create MipMapped Texture
				glBindTexture(GL_TEXTURE_2D, texture[2]);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

                glBindTexture(GL_TEXTURE_2D, texture[3]);
		        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);

        }

		for (int loop=0; loop<2; loop++)
		{
	        if (TextureImage[loop])							// If Texture Exists
		    {
			        if (TextureImage[loop]->data)			// If Texture Image Exists
				    {
					        free(TextureImage[loop]->data);	// Free The Texture Image Memory
					}
					free(TextureImage[loop]);				// Free The Image Structure
			}
		}
        return Status;                                  // Return The Status
}


//moves the current piece on the board and checks if game is over
int MoveCurrentPiece()
{
//	int matrix[PieceMaxRows][PieceMaxRows];
	int result = 0; //default : piece moved

	if (!MyBoard.MovePiece(MyPieces.currentPiece, 0)) //wait for one more cycle to store the piece on the table
	{
		pieceNotMoved ++;
		result = -1; //piece not moved because is about to store it
		if (pieceNotMoved == 1)
		{
			MyBoard.PlacePiece(MyPieces.currentPiece);
			MyBoard.EraseExistingLines(MyPieces.currentPiece->_y, MyPieces.currentPiece->_height);
			

			CreateNewPiece();
			if (MyBoard.CheckGameOver(MyPieces.currentPiece))
				gameOver = true;
			
			pieceNotMoved = 0;
			result = -2; // piece stored successfully
		}
	}

	return result;
}


GLvoid KillFont(GLvoid)									// Delete The Font From Memory
{
	glDeleteLists(base,256);							// Delete All 256 Display Lists
}


GLvoid BuildFont(GLvoid)										// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord

	base=glGenLists(256);								// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, texture[3]);			// Select Our Font Texture
	for (int loop=0; loop<256; loop++)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);							// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(10,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}

GLvoid glPrint(GLint x, GLint y, int set, char *string, ... )	// Where The Printing Happens
{
	
	char		text[256];										// Holds Our String
	va_list		ap;												// Pointer To List Of Arguments

	if (string == NULL)											// If There's No Text
		return;													// Do Nothing

	va_start(ap, string);										// Parses The String For Variables
	    vsprintf_s(text, string, ap);								// And Converts Symbols To Actual Numbers
	va_end(ap);													// Results Are Stored In Text


	if (set>1)
	{
		set=1;
	}
	glBindTexture(GL_TEXTURE_2D, texture[3]);			// Select Our Font Texture
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(0,800,0,600,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	glTranslated(x,y,0);								// Position The Text (0,0 - Bottom Left)
	glListBase(base-32+(128*set));						// Choose The Font Set (0 or 1)
	glCallLists(strlen(text),GL_UNSIGNED_BYTE,text);// Write The Text To The Screen
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}


//handles the key press
void Update (DWORD milliseconds, HDC hDC)								// Perform Motion Updates Here
{
	int matrix[PieceMaxRows][PieceMaxRows];

	if (g_keys->keyDown [VK_ESCAPE] == TRUE)					// Is ESC Being Pressed?
	{
		TerminateApplication (g_window);						// Terminate The Program
	}
	if (g_keys->keyDown [VK_F1] == TRUE)						// Is F1 Being Pressed?
	{
		ToggleFullscreen (g_window);							// Toggle Fullscreen Mode
	}
 
	if (g_keys->keyDown[VK_UP] && !upp)				// Is Up Arrow Being Pressed
	{
		
		//ypos -= g_PieceHeight;
		if (!gameOver)
		{
			MyBoard.GetPieceSurroundMatrix(MyPieces.currentPiece->_x, MyPieces.currentPiece->_y, matrix);
			MyPieces.RotatePiece(matrix);
			upp = TRUE;
		}
	}
	
	if (g_keys->keyDown[VK_UP] == FALSE)					// Has up Key Been Released?
	{
		upp = FALSE;				// If So, upp Becomes FALSE
	}
		


	if (g_keys->keyDown[VK_DOWN])				// Is Down Arrow Being Pressed
	{
		if (!gameOver)
		{
			MoveCurrentPiece();
		}
	}

	if (g_keys->keyDown[VK_PRIOR])				// Is Page Up Being Pressed
	{
		zpos -= 0.2f;
	}

	if (g_keys->keyDown[VK_NEXT])				// Is Page Down Being Pressed
	{
		zpos += 0.2f;
	}

	if (g_keys->keyDown[VK_RIGHT])				// Is Right Key Pressed
	{
		//xpos -= g_PieceWidth;
		if (!gameOver)
			MyBoard.MovePiece(MyPieces.currentPiece, 2);
	}

	if (g_keys->keyDown[VK_LEFT])				// Is Left Key Being Pressed
	{
		//xpos += g_PieceWidth;
		if (!gameOver)
			MyBoard.MovePiece(MyPieces.currentPiece, 1);
	}


	if (g_keys->keyDown['L'] == TRUE && !lp)				// L Key Being Pressed Not Held?
	{

   		lp=TRUE;				// lp Becomes TRUE
		light=!light;				// Toggle Light TRUE/FALSE

		if (!light)				// If Not Light
		{
			glDisable(GL_LIGHTING);		// Disable Lighting
		}
		else					// Otherwise
		{
			glEnable(GL_LIGHTING);		// Enable Lighting
		}
	}


	if (g_keys->keyDown['L'] == FALSE)	// Has L Key Been Released?
	{
		lp=FALSE;				// If So, lp Becomes FALSE
	}



	if (g_keys->keyDown['F'] == TRUE && !fp)				// Is F Key Being Pressed?
	{
		fp=TRUE;				// fp Becomes TRUE
		filter+=1;				// filter Value Increases By One
		if (filter>2)				// Is Value Greater Than 2?
		{
			filter=0;			// If So, Set filter To 0
		}
	}

	if (g_keys->keyDown['F'] == FALSE)					// Has F Key Been Released?
	{
		fp=FALSE;				// If So, fp Becomes FALSE
	}


	if (g_keys->keyDown['B'] && !bp)				// Is B Key Pressed And bp FALSE?
	{
		bp=TRUE;				// If So, bp Becomes TRUE
		blend = !blend;				// Toggle blend TRUE / FALSE	
		if(blend)				// Is blend TRUE?
		{
			glEnable(GL_BLEND);		// Turn Blending On
			glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
		}
		else					// Otherwise
		{
			glDisable(GL_BLEND);		// Turn Blending Off
			glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
		}
	}
	if (!g_keys->keyDown['B'])					// Has B Key Been Released?
	{
		bp=FALSE;				// If So, bp Becomes FALSE
	}

	if (g_keys->keyDown[VK_HOME])		// Is Home Key Being Pressed
	{
		xrot += 5.0f;
	}

	if (g_keys->keyDown[VK_END])		// Is END Key Being Pressed
	{
		xrot -= 5.0f;
	}


	if (g_keys->keyDown[VK_RETURN])		// Is Enter Key Being Pressed
	{
		if (gameOver)
		{
			gameOver = false;
			LocalInit();
		}
	}


	if (g_keys->keyDown[VK_SPACE] && !key_space_pressed)	// Is Space Key Being Pressed, not held?
	{
		gamePaused = !gamePaused;
		key_space_pressed = true;
	}
	
	if (!g_keys->keyDown[VK_SPACE])					// Has Space Key Been Released?
	{
		key_space_pressed = FALSE;				// If So, key_space_pressed Becomes FALSE
	}



	if (g_keys->keyPress)			// Was a key pressed ? 
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear Screen And Depth Buffer		
	}



	if (!gameOver && !gamePaused)
	{
		updateCycle ++;
		if (updateCycle >= MyBoard.dropSpeed) //drop speed
		{
			MoveCurrentPiece();
			updateCycle = 0;
		}
	}
	
	DrawGLScene();	// Draw The Scene
	

	glFlush ();													// Flush The GL Rendering Pipeline
	SwapBuffers (hDC);
	Sleep(60);													// Create A Short Delay (60 Milliseconds)
}

