#include "Application.h"
#include "Globals.h"
#include "ModuleHardware.h"

/* Video Memory */
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX			0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX		0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX    0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX				0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX				0x904B

ModuleHardware::ModuleHardware(Application* app, bool start_enabled) : Module(app, start_enabled)
{}
ModuleHardware::~ModuleHardware()
{}

bool ModuleHardware::Start()
{
	active = true;

	SDL_VERSION(&currentSDLVersion);

	cpus = SDL_GetCPUCount();
	cache = SDL_GetCPUCacheLineSize();
	
	systemRam = SDL_GetSystemRAM();
	systemRam *= 0.001;

	avx = SDL_HasAVX();
	rdtsc = SDL_HasRDTSC();
	mmx = SDL_HasMMX();
	sse = SDL_HasSSE();
	sse2 = SDL_HasSSE2();
	sse3 = SDL_HasSSE3();
	sse41 = SDL_HasSSE41();
	sse42 = SDL_HasSSE42();

	vendor = glGetString(GL_VENDOR);

	gpuName = glGetString(GL_RENDERER);

	glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);

	return true;
}
update_status ModuleHardware::Update(float dt)
{
	
	glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableMemory);

	glGetIntegerv(GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &dedicatedMemory);
	
	memoryUsage = totalMemory - availableMemory;

	return UPDATE_CONTINUE;
}
bool ModuleHardware::CleanUp()
{
	return true;
}

//Function that draws all the console
void ModuleHardware::Draw(const char* title)
{
	
	ImGui::Text("SDL Version: ");
	char SDLVer[256];
	sprintf_s(SDLVer, sizeof(SDLVer), "%d.%d.%d", currentSDLVersion.major, currentSDLVersion.minor, currentSDLVersion.patch);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), SDLVer);
	ImGui::Separator();


	ImGui::Text("Platform: ");
	const char* platform = SDL_GetPlatform();
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), platform);

	
	ImGui::Text("CPUs: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%i (Cache: %ikb)", cpus, cache);

	
	ImGui::Text("System RAM: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%iGb", systemRam);

	
	ImGui::Text("Caps: ");
	ImGui::SameLine();
	if (rdtsc == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "RDTSC");
		ImGui::SameLine();
	}
	if (mmx == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "MMX");
		ImGui::SameLine();
	}
	if (sse == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "SSE");
		ImGui::SameLine();
	}
	if (sse2 == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "SSE2");
		ImGui::SameLine();
	}
	if (sse3 == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "SSE3");
		ImGui::SameLine();
	}
	if (sse41 == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "SSE41");
		ImGui::SameLine();
	}
	if (sse42 == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "SSE42");
		ImGui::SameLine();
	}
	if (avx == true) {
		ImGui::TextColored(ImVec4(0, 1, 0, 100), "AVX");
	}
	ImGui::Separator();


	ImGui::Text("GPU: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", vendor);
	
	ImGui::Text("Brand: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%s", gpuName);

	ImGui::Text("VRAM Budget: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%.2f Mb", (totalMemory * 0.001));

	ImGui::Text("VRAM Usage: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%.2f Mb", (memoryUsage * 0.001));

	ImGui::Text("VRAM Available: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%.2f Mb", (availableMemory * 0.001));
	
	ImGui::Text("VRAM Reserved: ");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 100), "%.2f Mb", (dedicatedMemory * 0.001));

}