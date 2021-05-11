cd libdansdl
if [ -d objects_motor_sdl ];
then
	rm -rf objects_motor_sdl
fi
mkdir -p objects_motor_sdl
make -f makefile_motor_sdl -j4
cp libDanSDL.a ../
cd ..
if [ -d objects];
then
	rm -rf objects
fi
mkdir -p objects
make -j4
