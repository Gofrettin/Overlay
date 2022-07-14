#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <d3d11.h>

//ImFont* normalFont;
//ImFont* customFont;

namespace globals
{
	const char* const COMPILED = "Build: " __DATE__ " @ " __TIME__;
	

	

	//aimbot
	class aimbot
	{
	public:
		bool aimbotEnabled = false;
		bool smoothEnabled = false;
		bool accuracyEnabled = false;
		bool silentAimEnabled = false;
		bool rageModeEnabled = false;

	};

	class aimbotSettings
	{
	public:
		//int fovSlider = ImGui::SliderFloat("", &f1, 0.0f, 100.0f, "FOV Size = %.3f");
		float fovSize;
		float smoothAmount;
		const char* hitbox[4] = {"Head", "Neck", "Body","Leg"};
		int selectedHitbox = 0;



	};
		
	class visuals
	{
	public:
		bool visualsEnabled = false;
		bool boxEnabled = false;
		bool cornerEnabled = false;
		bool skeletonEnabled = false;
		bool glowEnabled = false;
		bool lineEnabled = false;
		bool headEnabled = false;
		bool triangleTracerEnabled = false;
	};

	class visualSettings
	{
	public:
		//int fovSlider = ImGui::SliderFloat("", &f1, 0.0f, 100.0f, "FOV Size = %.3f");
		float distance;
		float lineThickness;
		//const char* hitbox[4] = { "Head", "Neck", "Body","Leg" };
		//int selectedHitbox = 0;
		



	};
}
