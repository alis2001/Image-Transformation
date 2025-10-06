#include "ImageTransform.h"
#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include <cmath>

using namespace uiuc;

PNG grayscale(PNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      pixel.s = 0;
    }
  }
  return image;
}

PNG createSpotlight(PNG image, int centerX, int centerY) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      
      double dx = x - centerX;
      double dy = y - centerY;
      double distance = sqrt(dx * dx + dy * dy);
      
      double decrease = distance * 0.005;
      if (decrease > 0.8) {
        decrease = 0.8;
      }
      
      pixel.l = pixel.l * (1 - decrease);
    }
  }
  return image;
}

PNG illinify(PNG image) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      
      double illiniOrange = 11;
      double illiniBlue = 216;
      
      double distToOrange = fabs(pixel.h - illiniOrange);
      double distToBlue = fabs(pixel.h - illiniBlue);
      
      if (distToOrange > 180) {
        distToOrange = 360 - distToOrange;
      }
      if (distToBlue > 180) {
        distToBlue = 360 - distToBlue;
      }
      
      if (distToOrange < distToBlue) {
        pixel.h = illiniOrange;
      } else {
        pixel.h = illiniBlue;
      }
    }
  }
  return image;
}

PNG watermark(PNG firstImage, PNG secondImage) {
  unsigned width = firstImage.width();
  unsigned height = firstImage.height();
  
  if (secondImage.width() < width) {
    width = secondImage.width();
  }
  if (secondImage.height() < height) {
    height = secondImage.height();
  }
  
  for (unsigned x = 0; x < width; x++) {
    for (unsigned y = 0; y < height; y++) {
      HSLAPixel & basePixel = firstImage.getPixel(x, y);
      HSLAPixel & stencilPixel = secondImage.getPixel(x, y);
      
      if (stencilPixel.l == 1.0) {
        basePixel.l = basePixel.l + 0.2;
        if (basePixel.l > 1.0) {
          basePixel.l = 1.0;
        }
      }
    }
  }
  return firstImage;
}