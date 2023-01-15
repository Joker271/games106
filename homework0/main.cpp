#include "stdafx.h"
#include "json.h"
#include "base/DXCHelper.h"
#include "base/FrameworkWindows.h"
#include "Base/ShaderCompilerHelper.h"
#include "Base/CommandListRing.h"

using namespace CAULDRON_VK;
using namespace nlohmann;
// We are queuing (backBufferCount + 0.5) frames, so we need to triple buffer the resources that get modified each frame
constexpr int backBufferCount = 3;
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

    auto createRenderPass() -> void;
    auto createGraphicsPipeline() -> void;
    auto createFramebuffers() -> void;
    auto createCommandPool() -> void;
    auto createCommandBuffer() -> void;
    auto createSyncObjects() -> void;

    CommandListRing m_CommandListRing;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

Homework0Sample::Homework0Sample(LPCSTR name) :
    FrameworkWindows(name)
{
}

auto Homework0Sample::createRenderPass() -> void
{
}
auto Homework0Sample::createGraphicsPipeline() -> void
{
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};

    if (VKCompileFromFile(
            m_device.GetDevice(),
            VK_SHADER_STAGE_VERTEX_BIT,
            "homework0/hello_triangle_vert.glsl",
            "main", "",
            nullptr, &vertShaderStageInfo) != VkResult::VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex shader!");
    }

    if (VKCompileFromFile(
            m_device.GetDevice(),
            VK_SHADER_STAGE_FRAGMENT_BIT,
            "homework0/hello_triangle_frag.glsl",
            "main", "",
            nullptr, &fragShaderStageInfo) != VkResult::VK_SUCCESS)
    {
        throw std::runtime_error("failed to create fragment shader!");
    }

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(m_device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = m_swapChain.GetRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(m_device.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(m_device.GetDevice(), vertShaderStageInfo.module, nullptr);
    vkDestroyShaderModule(m_device.GetDevice(), vertShaderStageInfo.module, nullptr);
}
auto Homework0Sample::createFramebuffers() -> void
{
}


auto Homework0Sample::createCommandPool() -> void
{
}
auto Homework0Sample::createCommandBuffer() -> void
{
}
auto Homework0Sample::createSyncObjects() -> void
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
    // Create a commandlist ring for the Direct queue
    constexpr uint32_t commandListsPerBackBuffer = 8;

    m_CommandListRing.OnCreate(&m_device, backBufferCount, commandListsPerBackBuffer);

    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createCommandPool();
    createCommandBuffer();
    createSyncObjects();

    OnResize(true);
    OnUpdateDisplay();
}
void Homework0Sample::OnDestroy()
{
    m_device.GPUFlush();
    m_CommandListRing.OnDestroy();
    // shut down the shader compiler
    DestroyShaderCache(&m_device);
}

void Homework0Sample::OnRender()
{
    // Do any start of frame necessities
    BeginFrame();
    int imageIndex = m_swapChain.WaitForSwapChain();

    m_CommandListRing.OnBeginFrame();
    VkCommandBuffer commandBuffer = m_CommandListRing.GetNewCommandList();

    vkResetCommandBuffer(commandBuffer, /*VkCommandBufferResetFlagBits*/ 0);

    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_swapChain.GetRenderPass();
        renderPassInfo.framebuffer = m_swapChain.GetCurrentFramebuffer();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = {GetWidth(), GetHeight()};

        VkClearValue clearColor = {{{0.0f, 1.0f, 1.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)GetWidth();
        viewport.height = (float)GetHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = {GetWidth(), GetHeight()};
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }


    VkSemaphore ImageAvailableSemaphore;
    VkSemaphore RenderFinishedSemaphores;
    VkFence CmdBufExecutedFences;
    m_swapChain.GetSemaphores(&ImageAvailableSemaphore, &RenderFinishedSemaphores, &CmdBufExecutedFences);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {ImageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = {RenderFinishedSemaphores};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(m_device.GetGraphicsQueue(), 1, &submitInfo, CmdBufExecutedFences) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

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