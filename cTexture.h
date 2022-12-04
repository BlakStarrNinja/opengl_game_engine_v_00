#ifndef	_CTEXTURE_H_
#define	_CTEXTURE_H_

class cTexture
{
	protected:
//		cGraphics		*_pGraphics;
		unsigned long	_width, _height;
		unsigned char	_bPP;				//texels bits per pixel

		unsigned long	_textureID;

	public:
		cTexture();
//		cTexture(cGraphics *pGraphics, char *filename);
		cTexture(char *filename);
		~cTexture();

//		bool	_Load(cGraphics *pGraphics, char *filename);
		bool	_Load(const char *filename);
//		bool	_Create(cGraphics *pGraphics, unsigned long width, unsigned long height);
		bool	_Free(void);

//		bool	_Activate(bool bactivateTexture);
		bool	_Bind(bool bbindTexture);

		bool	_IsLoaded(void);

		unsigned long	_GetWidth(void);
		unsigned long	_GetHeight(void);
		unsigned char _GetBPP(void);
/*
		bool	_Blit(	long destX, long destY,
						long srcX = 0, long srcY = 0,
						long width = 0, long height = 0,
						float scaleX = 1.0f, float scaleY = 1.0f);
*/
};

#endif	// _CTEXTURE_H_
