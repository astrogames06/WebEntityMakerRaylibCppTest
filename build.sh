FILES=$(find src/ -name "*.cpp")

em++ -o index.js $FILES -std=c++20 -Os -Wall ./lib/libraylib.web.a -I. -I include/ -L. -L lib/ -lembind -s USE_GLFW=3 --preload-file assets/ -DPLATFORM_WEB16