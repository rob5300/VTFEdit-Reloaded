#pragma once
#include <windows.h>
#include <wtypes.h>
#include <stdio.h>
#include <string>
#include "IL\il.h"
#include "VTFFile.h"

using namespace VTFLib;
using namespace std;

enum CubemapFaces
{
	TOP,
	LEFT,
	FORWARD,
	RIGHT,
	BACK,
	BOTTOM
};

string CubemapFaceNames[6] = {
	"up.vtf", "lf.vtf", "ft.vtf", "rt.vtf", "bk.vtf", "dn.vtf"
};

//Ported from: https://github.com/Mapiarz/CubemapToEquirectangular
bool CreateEquirectangularImage(string* &faces, int channels = 4, int bytesPerColour = 1, bool hdr = true)
{
    CVTFFile faceVTFs[6];
    int cubeFaceWidth;
    int cubeFaceHeight;

    for (int i = 0; i < 6; i++)
    {
        auto success = faceVTFs[i].Load(faces[i].c_str());
        if (success)
        {
            if()

            if (i == 0)
            {
                cubeFaceWidth = faceVTFs[0].GetWidth();
                cubeFaceHeight = faceVTFs[0].GetHeight();
            }
            else
            {
                int width = faceVTFs[i].GetWidth();
                int height = faceVTFs[i].GetHeight();
                if (cubeFaceWidth != width || cubeFaceHeight != height)
                {
                    printf("Error: All cube map faces must have the same size");
                    return false;
                }
            }
        }
        else
        {
            printf("Error: Unable to load image '%s'.", faces[i]);
            return false;
        }
    }
    
    ILuint equiTexture;
    int outputWidth = 2048;
    int outputHeight = 1024;
    ilBindImage(equiTexture);
    //Define new image presuming source images are BGRA8888.
    ilTexImage(outputWidth, outputHeight, 1, 4, IL_BGRA, IL_UNSIGNED_SHORT, NULL);
        
    /*
	Texture2D equiTexture = new Texture2D(outputWidth, outputHeight, TextureFormat.ARGB32, false);
    float u, v; //Normalised texture coordinates, from 0 to 1, starting at lower left corner
    float phi, theta; //Polar coordinates
    int cubeFaceWidth, cubeFaceHeight;

    cubeFaceWidth = sourceTexture.width / 4; //4 horizontal faces
    cubeFaceHeight = sourceTexture.height / 3; //3 vertical faces

    
    for (int j = 0; j < equiTexture.height; j++)
    {
        //Rows start from the bottom
        v = 1 - ((float)j / equiTexture.height);
        theta = v * Mathf.PI;

        for (int i = 0; i < equiTexture.width; i++)
        {
            //Columns start from the left
            u = ((float)i / equiTexture.width);
            phi = u * 2 * Mathf.PI;

            float x, y, z; //Unit vector
            x = Mathf.Sin(phi) * Mathf.Sin(theta) * -1;
            y = Mathf.Cos(theta);
            z = Mathf.Cos(phi) * Mathf.Sin(theta) * -1;

            float xa, ya, za;
            float a;

            a = Mathf.Max(new float[3] { Mathf.Abs(x), Mathf.Abs(y), Mathf.Abs(z) });

            //Vector Parallel to the unit vector that lies on one of the cube faces
            xa = x / a;
            ya = y / a;
            za = z / a;

            Color color;
            int xPixel, yPixel;
            int xOffset, yOffset;

            if (xa == 1)
            {
                //Right
                xPixel = (int)((((za + 1f) / 2f) - 1f) * cubeFaceWidth);
                xOffset = 2 * cubeFaceWidth; //Offset
                yPixel = (int)((((ya + 1f) / 2f)) * cubeFaceHeight);
                yOffset = cubeFaceHeight; //Offset
            }
            else if (xa == -1)
            {
                //Left
                xPixel = (int)((((za + 1f) / 2f)) * cubeFaceWidth);
                xOffset = 0;
                yPixel = (int)((((ya + 1f) / 2f)) * cubeFaceHeight);
                yOffset = cubeFaceHeight;
            }
            else if (ya == 1)
            {
                //Up
                xPixel = (int)((((xa + 1f) / 2f)) * cubeFaceWidth);
                xOffset = cubeFaceWidth;
                yPixel = (int)((((za + 1f) / 2f) - 1f) * cubeFaceHeight);
                yOffset = 2 * cubeFaceHeight;
            }
            else if (ya == -1)
            {
                //Down
                xPixel = (int)((((xa + 1f) / 2f)) * cubeFaceWidth);
                xOffset = cubeFaceWidth;
                yPixel = (int)((((za + 1f) / 2f)) * cubeFaceHeight);
                yOffset = 0;
            }
            else if (za == 1)
            {
                //Front
                xPixel = (int)((((xa + 1f) / 2f)) * cubeFaceWidth);
                xOffset = cubeFaceWidth;
                yPixel = (int)((((ya + 1f) / 2f)) * cubeFaceHeight);
                yOffset = cubeFaceHeight;
            }
            else if (za == -1)
            {
                //Back
                xPixel = (int)((((xa + 1f) / 2f) - 1f) * cubeFaceWidth);
                xOffset = 3 * cubeFaceWidth;
                yPixel = (int)((((ya + 1f) / 2f)) * cubeFaceHeight);
                yOffset = cubeFaceHeight;
            }
            else
            {
                Debug.LogWarning("Unknown face, something went wrong");
                xPixel = 0;
                yPixel = 0;
                xOffset = 0;
                yOffset = 0;
            }

            xPixel = Mathf.Abs(xPixel);
            yPixel = Mathf.Abs(yPixel);

            xPixel += xOffset;
            yPixel += yOffset;

            color = sourceTexture.GetPixel(xPixel, yPixel);
            equiTexture.SetPixel(i, j, color);
        }
    }

    equiTexture.Apply();
    var bytes = equiTexture.EncodeToPNG();
    Object.DestroyImmediate(equiTexture);

    return bytes;
    */
}

string* LocateCubemapFaces(char* &folder)
{
	string cubemapFacesPaths[6];
	char lpSearchString[512];
	char lpPath[512];

	WIN32_FIND_DATA FindData;
	HANDLE Handle;

	sprintf(lpSearchString, "%s\\%s", folder, "*.vtf");

	Handle = FindFirstFile(lpSearchString, &FindData);

	if(Handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(stricmp(FindData.cFileName, ".") != 0 && stricmp(FindData.cFileName, "..") != 0)
			{
				sprintf(lpPath, "%s\\%s", folder, FindData.cFileName);

				if((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
				{
					int face = FindFaceForFilename(FindData.cFileName);
					if (face != -1)
					{
						cubemapFacesPaths[face] = string(FindData.cFileName);
					}
				}
			}
		} while(FindNextFile(Handle, &FindData));

		FindClose(Handle);
	}

	return cubemapFacesPaths;
}

int FindFaceForFilename(char* filename)
{
	string name(filename);
	for (int i = 0; i < 6, i++;)
	{
		string* ending = &CubemapFaceNames[i];
		if (std::equal(ending->rbegin(), ending->rend(), name.rbegin()))
		{
			return i;
		}
	}
	return -1;
}

vlByte* GetPixel(int& x, int& y, int& width, int& height, int &channels, int &bytesPerPixel, vlByte*& data)
{

}