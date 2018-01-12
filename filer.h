#ifndef FILER_H
#define FILER_H

// header file FILER.H for class filer
// a class object can read file

class Filer
{
public:
  unsigned char* ppmRead( char* filename, int* width, int* height );

};

#endif //FILER_H