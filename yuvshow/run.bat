cmake -DCMAKE_TOOLCHAIN_FILE=D:\work_home\Toolcode\yuvshow\android.toolchain.cmake -DANDROID_NDK=D:\Green_Soft\SDK\ndk-bundle -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a with NEON" D:\work_home\Toolcode\yuvshow -DCMAKE_GENERATOR="Ninja"

cmake --build .

pause