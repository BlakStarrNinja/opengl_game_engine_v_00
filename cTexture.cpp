#include "core_global.h"

cTexture::cTexture()
{
//	_pGraphics = NULL;
	_width = _height = _textureID =
	_bPP = 0;
}

cTexture::cTexture(char *filename)
{
//	_pGraphics = NULL;
	_width = _height = _textureID =
	_bPP = 0;

	_Load(filename);
}

cTexture::~cTexture() { _Free(); }

bool
cTexture::_Load(const char *filename)
{

	ILuint imgID;

	ilGenImages(1, &imgID);

	ilBindImage(imgID);

	if (!ilLoadImage(gILConvertMBtoWC(filename)))
//    if (!ilLoad(IL_TGA, gILConvertMBtoWC(filename)))
	{
		ilDeleteImages(1, &imgID);

		int nerror = ilGetError();

//		std::string ilerror = iluErrorString(nerror);
		const char* ilerror = gILConvertWCtoMB(iluErrorString(nerror));

		std::string sfilename = filename;
		std::string errorMsg = "Error: \nCould NOT open file \"" + sfilename + "\"!\nPlease check to make sure file exists.\nTextue is not created.";
		_MB(errorMsg.c_str(), "ERROR:	INVALID IMAGE FILE");
		_MB(ilerror, "OPENIL Error");

		return false;
	}

	////set correct IL image starting origin
	if (ilGetInteger(IL_ORIGIN_MODE) == IL_ORIGIN_LOWER_LEFT)
    {
        cout<<"Origin of image file \""<< filename <<"\" is Lower Left."<<endl;
    }
    else
        cout<<"Origin of image file \""<< filename <<"\" is Upper Left."<<endl;

	////

	_width	= ilGetInteger(IL_IMAGE_WIDTH);
	_height = ilGetInteger(IL_IMAGE_HEIGHT);
	_bPP	= ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);

    unsigned	internalFormat = 0,
                imageFormat = 0,
                imageDataType = 0;

    switch (ilGetInteger(IL_IMAGE_FORMAT))
    {
        case IL_COLOUR_INDEX:
            /////notice something wrong with this format.  added the next to lines to see if this format works for system.
            /////the two lines below started up without and error, but texture still did not appear.
            /////closing this out as an unsupported texture
            /*internalFormat = GL_RGB;
            imageFormat = GL_RGB_INTEGER;
            */
            ilDeleteImages(1, &imgID);
            cout<<"color_index"<<endl;
            cout<<"unsupported image format."<<endl;

            return false;
        break;

        case IL_RGB:
            internalFormat = GL_RGB;
            imageFormat = GL_RGB;
            cout<<"RGB"<<endl;
        break;

        case IL_RGBA:
            internalFormat = GL_RGBA;
            imageFormat = GL_RGBA;
            cout<<"RGBA"<<endl;
        break;

        case IL_BGR:
            internalFormat = GL_RGB;
            imageFormat = GL_BGR;
            cout<<"BGR"<<endl;
        break;

        case IL_BGRA:
            internalFormat = GL_RGBA;
            imageFormat = GL_BGRA;
            cout<<"BGRA"<<endl;
        break;

        case IL_LUMINANCE:
            ilDeleteImages(1, &imgID);
            cout<<"LUMINANCE"<<endl;
            cout<<"unsupported image format."<<endl;

            return false;
        break;

        default:
            ilDeleteImages(1, &imgID);
            cout<<"unknown format"<<endl;
            cout<<"unsupported image format."<<endl;

            return false;
    }

/*
    switch (ilGetInteger(IL_IMAGE_FORMAT))
    {
        case IL_RGB:
            internalFormat = GL_RGB;
            imageFormat = GL_RGB;
        break;

        case IL_RGBA:
            internalFormat = GL_RGBA;
            imageFormat = GL_RGBA;
        break;

        case IL_BGR:
            internalFormat = GL_RGB;
            imageFormat = GL_BGR;
        break;

        case IL_BGRA:
            internalFormat = GL_RGBA;
            imageFormat = GL_BGRA;
        break;

        default:
        {
            ilDeleteImages(1, &imgID);
            std::string sfilename = filename;
            std::string errorMsg = "The file, \"" + sfilename + "\", is in a format that is not supported.\nOnly normalized integer or floating point based formats are supported.Palette textures are not supported.\nTextue is not created.";
            _MB(errorMsg.c_str(), "ERROR:	UNSUPPORTED IMAGE FILE FORMAT");

            return false;
        }
    }
*/
    switch (ilGetInteger(IL_IMAGE_TYPE))
    {
        case IL_BYTE:
            imageDataType = GL_BYTE;
        break;

        case IL_UNSIGNED_BYTE:
            imageDataType = GL_UNSIGNED_BYTE;
        break;

        case IL_SHORT:
            imageDataType = GL_SHORT;
        break;

        case IL_UNSIGNED_SHORT:
            imageDataType = GL_UNSIGNED_SHORT;
        break;

        case IL_INT:
            imageDataType = GL_INT;
        break;

        case IL_UNSIGNED_INT:
            imageDataType = GL_UNSIGNED_INT;
        break;

        case IL_FLOAT:
            imageDataType = GL_FLOAT;
        break;

        case IL_DOUBLE:
            imageDataType = GL_DOUBLE;
        break;
    }

    glGenTextures(1, (GLuint*)&_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(							//supplies opengl with image info and data
        GL_TEXTURE_2D,						//texture target
        0,									//level-of-detail or mipmap level. 0 stands for base image.
        internalFormat,						//data format for opengl
        ilGetInteger(IL_IMAGE_WIDTH),		//image-now textures- width
        ilGetInteger(IL_IMAGE_HEIGHT),		//height
        0,									//!!DEPRECATED!!ALWAYS SET TO 0!! size of border to create around texture
        imageFormat,						//image's format
        imageDataType,						//image's data will be this type
        (void*)ilGetData() );				//pointer to image's data

    glGenerateMipmap(GL_TEXTURE_2D);

	ilDeleteImages(1, &imgID);

	if (!_textureID)
    {
        glDeleteTextures(1, (GLuint*)&_textureID);
        std::string errorMsg = "For some unknown reason, the textue could not be created.";
        _MB(errorMsg.c_str(), "ERROR:	TEXTRUE CREATION ERROR");

        return false;
    }

	return true;
}

bool
cTexture::_Free(void)
{
//	_Bind(true);

	if (_textureID)
	{
		glDeleteTextures(1, (const unsigned int *)&_textureID);

//		_pGraphics = NULL;
		_width = _height = _textureID =
		_bPP = 0;
	}

	return true;
}

bool
cTexture::_Bind(bool bbindTexture)
{
	(bbindTexture) ? (glBindTexture(GL_TEXTURE_2D, _textureID)) :
						(glBindTexture(GL_TEXTURE_2D, 0));

	return true;
}

bool
cTexture::_IsLoaded(void)
{
	if (_textureID)	return true;

	return false;
}

unsigned long
cTexture::_GetWidth(void) { return _width; }

unsigned long
cTexture::_GetHeight(void) { return _height; }

unsigned char
cTexture::_GetBPP(void) { return _bPP; }
