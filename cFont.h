#ifndef _CFONT_H_
#define _CFONT_H_

class cCharacter
{
    protected:
		int _id;
		double _uTextureCoord;
		double _vTextureCoord;
		double _uMaxTextureCoord;
		double _vMaxTextureCoord;
		double _xOffset;
		double _yOffset;
		double _sizeX;
		double _sizeY;
		double _xAdvance;

	public:
		cCharacter();
		~cCharacter();
		cCharacter(int charID, double uTexCoord, double vTexCoord, double uMaxTexCoord, double vMaxTexCoord,
					double xOff, double yOff, double xSize, double ySize, double xAdv);

		inline int _GetID() const {	return _id; }
		inline double _GetUTexCoord() const {	return _uTextureCoord; }
		inline double _GetVTexCoord() const {	return _vTextureCoord; }
		inline double _GetUMaxTexCoord() const {	return _uMaxTextureCoord; }
		inline double _GetVMaxTexCoord() const {	return _vMaxTextureCoord; }
		inline double _GetxOff() const {	return _xOffset; }
		inline double _GetyOff() const {	return _yOffset; }
		inline double _GetSizeX() const {	return _sizeX; }
		inline double _GetSizeY() const {	return _sizeY; }
		inline double _GetXAdv() const {	return _xAdvance; }

		void _SetCharacter(int charID, double uTexCoord, double vTexCoord, double uMaxTexCoord, double vMaxTexCoord,
					double xOff, double yOff, double xSize, double ySize, double xAdv);
};

class cWord
{
	protected:
		std::vector<cCharacter> _characters;
		double _width;
		double _fontSize;

	public:
		cWord();

};



class cFontData
{
	protected:
		std::unordered_map<int, cCharacter> _characterData;
		double _aspectRatio;
		double _verticalPerPixelSize;
		double _horizontalPerPixelSize;
		double _spaceWidth;

	protected:
		cCharacter _LoadCharacter(int imageWidth, int charID, int x, int y, int w, int h, int xOffset, int yOffset, int xAdvance);

	public:
		cFontData();
		~cFontData();

		inline cCharacter _GetCharacter(int ascii)
		{	return _characterData[ascii];	}

		inline double _GetAspectRatio()     {	return _aspectRatio;	}
		inline double _GetVerPerPixelSize() {	return _verticalPerPixelSize;	}
		inline double _GetHorPerPixelSize() {	return _horizontalPerPixelSize;	}
		inline double _GetSpaceWidth()      {	return _spaceWidth;	}

		void _SetAspectRatio(int screenWidth, int screenHeight);

		void _SetLineSizes(double lineHeight, int lineHeightinPx);
		void _SetSpaceWidth(int spaceXAdv);
		void _SetCharacterData(int imageWidth, int charID, int x, int y, int w, int h, int xOffset, int yOffset, int xAdvance);

		void _PrintCharacterData(int ascii);
};



class cFont
{
    protected:
		//cGraphics	*_pgraphics;


    public:
        cFont();
        virtual ~cFont();



};

#endif // _CFONT_H_
