#include "core_global.h"

#include <vector>

//cCharacter
cCharacter::cCharacter()
{
    _id = -1;
    _uTextureCoord = _vTextureCoord = _uMaxTextureCoord = _vMaxTextureCoord =
    _xOffset = _yOffset = _sizeX = _sizeY = _xAdvance = 0.0;
}

cCharacter::~cCharacter()
{
    _id = -1;
    _uTextureCoord = _vTextureCoord = _uMaxTextureCoord = _vMaxTextureCoord =
    _xOffset = _yOffset = _sizeX = _sizeY = _xAdvance = 0.0;
}

cCharacter::cCharacter(int charID, double uTexCoord, double vTexCoord,
                       double uMaxTexCoord, double vMaxTexCoord, double xOff,
                       double yOff, double xSize, double ySize, double xAdv)
{
    _id = charID;
    _uTextureCoord = uTexCoord;
    _vTextureCoord = vTexCoord;
    _uMaxTextureCoord = uMaxTexCoord;
    _vMaxTextureCoord = vMaxTexCoord;
    _xOffset = xOff;
    _yOffset = yOff;
    _sizeX = xSize;
    _sizeY = ySize;
    _xAdvance = xAdv;
}

void
cCharacter::_SetCharacter(int charID, double uTexCoord, double vTexCoord,
                          double uMaxTexCoord, double vMaxTexCoord, double xOff,
                          double yOff, double xSize, double ySize, double xAdv)
{
    _id = charID;
    _uTextureCoord = uTexCoord;
    _vTextureCoord = vTexCoord;
    _uMaxTextureCoord = uMaxTexCoord;
    _vMaxTextureCoord = vMaxTexCoord;
    _xOffset = xOff;
    _yOffset = yOff;
    _sizeX = xSize;
    _sizeY = ySize;
    _xAdvance = xAdv;

    return;
}


//cFontData
cCharacter
cFontData::_LoadCharacter(int imageWidth, int charID, int x, int y, int w, int h, int xOffset, int yOffset, int xAdvance)
{
    double uTexCoord = (double)x / (double)imageWidth;
    double vTexCoord = (double)(imageWidth - y) / (double)imageWidth;
    double uMaxTexCoord = (double)w / (double)imageWidth;
    double vMaxTexCoord = (double)-h / (double)imageWidth;
    double xOff = (double)xOffset * _horizontalPerPixelSize;
    double yOff = (double)yOffset * _verticalPerPixelSize; //may have to multiply this line by -1
    double xSize = w * _horizontalPerPixelSize;
    double ySize = h * _verticalPerPixelSize;
    double xAdv = xAdvance * _horizontalPerPixelSize;

    return cCharacter(charID, uTexCoord, vTexCoord, uMaxTexCoord, vMaxTexCoord, xOff, yOff, xSize, ySize, xAdv);
}



cFontData::cFontData()
{
    _characterData.clear();
    _aspectRatio = 1.0;
    _verticalPerPixelSize = .01;
    _horizontalPerPixelSize = .01;
    _spaceWidth = .1;
}

cFontData::~cFontData()
{
    _characterData.clear();
    _aspectRatio = 1.0;
    _verticalPerPixelSize = .01;
    _horizontalPerPixelSize = .01;
    _spaceWidth = .1;
}

void
cFontData::_SetAspectRatio(int screenWidth, int screenHeight)
{
    _aspectRatio = (double)screenWidth / (double)screenHeight;

    return;
}

void
cFontData::_SetLineSizes(double lineHeight, int lineHeightinPx)
{
    _verticalPerPixelSize = lineHeight / (double)lineHeightinPx;
    _horizontalPerPixelSize = _verticalPerPixelSize / _aspectRatio;

    return;
}

void
cFontData::_SetSpaceWidth(int spaceXAdv)
{
    _spaceWidth = (double)spaceXAdv * _horizontalPerPixelSize;

    return;
}

/*cCharacter(int charID, double uTexCoord, double vTexCoord, double uMaxTexCoord, double vMaxTexCoord,
			double xOff, double yOff, double xSize, double ySize, double xAdv)
*/
void
cFontData::_SetCharacterData(int imageWidth, int charID, int x, int y, int w, int h, int xOffset, int yOffset, int xAdvance)
{
    cCharacter c = _LoadCharacter(imageWidth, charID, x, y, w, h, xOffset, yOffset, xAdvance);
    _characterData[c._GetID()] = c;
}

void
cFontData::_PrintCharacterData(int ascii)
{
    cCharacter c = _GetCharacter(ascii);
    std::string str =
        "u:" + std::to_string(c._GetUTexCoord()) + ","
        "v:" + std::to_string(c._GetVTexCoord()) + ","
        "uM:" + std::to_string(c._GetUMaxTexCoord()) + ","
        "vM:" + std::to_string(c._GetVMaxTexCoord());
    std::cout<<(char)c._GetID()<<": "+str<<std::endl;
    str = std::string("   ") +
        "x:" + std::to_string(c._GetxOff()) + ","
        "y:" + std::to_string(c._GetyOff()) + ","
        "xM:" + std::to_string(c._GetSizeX()) + ","
        "yM:" + std::to_string(c._GetSizeY()) + "\n   " +
        "xAdv:" + std::to_string(c._GetXAdv());
    std::cout<<str<<std::endl;

    return;
}




//cWord
cWord::cWord()
{
    _characters.clear();
    _width = 0.0;
    _fontSize = 0.0;
}




//cFont
cFont::cFont()
{
    //ctor
}

cFont::~cFont()
{
    //dtor
}
