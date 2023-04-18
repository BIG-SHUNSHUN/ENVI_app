#ifndef IMAGEFACTORY_H
#define IMAGEFACTORY_H

#include "Image.h"
#include <QString>

//Image建造类
class ImageFactory
{
public:
    ImageFactory();
    static Image* CreateImage(string file);

private:
    //判断影像类型
    static IMAGETYPE DecideImageType(string& fileName);
};

#endif // IMAGEFACTORY_H
