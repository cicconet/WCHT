Wavelet-Based Circular Hough Transform

Helps finding circles of a particular radius in images.


main.cpp
  Demonstration of how to find circles of a particular radius.

Accumulator class
  Accumulates votes for centers.

Image class
  Image IO using the PNG library.

Wavelet class
  Morlet wavelet kernels.

Convolution class
  Convolves images with kernels.

LocMax class
  Locates local maxima in the accumulator space.

*.png files
  Sample images.

WCHT.xcodeproj
  Xcode project file, in case you use Xcode.
  To compile on Terminal, type
  g++ *.cpp -o wcht -I/usr/local/include -L/opt/local/lib -lpng
  or similar, depending on where the PNG library is installed in your machine.


Reference:
Marcelo C., Davi G., and Kris G.
Wavelet-Based Circular Hough Transform and its Application in Embryo Development Analysis.
VISAPP, Barcelona, 2013. BibTeX:
@proceedings{Cicconet2013,
  editor    = {Sebastiano Battiato and Jos{\'e} Braz},
  title     = {VISAPP 2013 - Proceedings of the International Conference
              on Computer Vision Theory and Applications, Volume 1, Barcelona,
              Spain, 21-24 February, 2013},
  booktitle = {VISAPP (1)},
  publisher = {SciTePress},
  year      = {2013},
  isbn      = {978-989-8565-47-1}
}


Marcelo Cicconet
New York University
marceloc.net