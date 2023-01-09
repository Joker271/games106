#include "stdafx.h"
#include "json.h"
#include "base/DXCHelper.h"
#include "base/FrameworkWindows.h"
#include "Base/ShaderCompilerHelper.h"
using namespace CAULDRON_VK;
using namespace nlohmann;
class Homework0Sample : public FrameworkWindows
{
public:
    Homework0Sample(LPCSTR name);
    void OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight) override;
    void OnCreate() override;
    void OnDestroy() override;
    void OnRender() override;
    bool OnEvent(MSG msg) override;
    void OnResize(bool resizeRender) override;
    void OnUpdateDisplay() override;
};

Homework0Sample::Homework0Sample(LPCSTR name) :
    FrameworkWindows(name)
{
}

void Homework0Sample::OnParseCommandLine(LPSTR lpCmdLine, uint32_t* pWidth, uint32_t* pHeight)
{
    // set some default values
    *pWidth = 1920;
    *pHeight = 1080;
    m_VsyncEnabled = false;

    // read globals
    auto process = [&](json jData) {
        *pWidth = jData.value("width", *pWidth);
        *pHeight = jData.value("height", *pHeight);
        m_fullscreenMode = jData.value("presentationMode", m_fullscreenMode);
        m_isCpuValidationLayerEnabled = jData.value("CpuValidationLayerEnabled", m_isCpuValidationLayerEnabled);
        m_isGpuValidationLayerEnabled = jData.value("GpuValidationLayerEnabled", m_isGpuValidationLayerEnabled);
        m_VsyncEnabled = jData.value("vsync", m_VsyncEnabled);
        m_FreesyncHDROptionEnabled = jData.value("FreesyncHDROptionEnabled", m_FreesyncHDROptionEnabled);
    };

    // read json globals from commandline
    //
    try
    {
        if (strlen(lpCmdLine) > 0)
        {
            auto j3 = json::parse(lpCmdLine);
            process(j3);
        }
    }
    catch (json::parse_error)
    {
        Trace("Error parsing commandline\n");
        exit(0);
    }
}

void Homework0Sample::OnCreate()
{
    // Init the shader compiler
    InitDirectXCompiler();
    CreateShaderCache();

    OnResize(true);
    OnUpdateDisplay();

    // Init Camera, looking at the origin
}
void Homework0Sample::OnDestroy()
{

    m_device.GPUFlush();

    // shut down the shader compiler
    DestroyShaderCache(&m_device);
}

void Homework0Sample::OnRender()
{
    // Do any start of frame necessities
    BeginFrame();

    // Framework will handle Present and some other end of frame logic
    EndFrame();
}

bool Homework0Sample::OnEvent(MSG msg)
{
    return true;
}
void Homework0Sample::OnResize(bool resizeRender) {}

void Homework0Sample::OnUpdateDisplay() {}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    LPCSTR Name = "Homework0Sample";

    // create new Homework0Sample  sample
    return RunFramework(hInstance, lpCmdLine, nCmdShow, new Homework0Sample(Name));
}