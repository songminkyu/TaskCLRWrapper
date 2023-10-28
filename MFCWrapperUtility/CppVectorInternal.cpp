
extern "C" __declspec(dllexport) GetNativeStructs(CppVectorInternal<NativeStruct>*out)
{
    std::uniform_int_distribution<int> dist1{ 0, 10 };
    std::normal_distribution<double> dist2;
    std::normal_distribution<float> dist3;

    std::random_device device;
    std::default_random_engine engine{ device() };

    std::vector<NativeStruct> result;
    for (int i{}; i < 20; ++i) {
        result.push_back(NativeStruct{
            dist1(engine),
            dist2(engine),
            dist3(engine),
            });
    }

    *out = result;
}