/*
	                 ,;;,.                  ,;'',
                    /~\                    /~\
                   ([~])                  ([~])
                 ,_.~~~.                  .~~~.
               ()--|   ,\                /    ,\    ()
            ,_//   |   |>)              (<|   |\()--'m
         (~'  m''~)(   )/                \(   )   ~~'|
          \(~||~)/ //~\\                  //~\\     ||
             ||   ()   ()                ()   () /( || )\
             ||   ||   ||                ||   ||( '-||-' )
             || ,;.)   (.;,            ,;.)   (.;,(~\/~)/

	These are the header guards. They guard every header in this project, keeping the lands of Nabi safe.
*/

#include "Main.h"

#include "Demo\Core\Demo.h"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
    nabi::NabiCoreSettings nabiCoreSettings = nabi::c_NabiCoreDefaultSettings;
    nabiCoreSettings.m_DataSettings.m_RouteDocument = "Tests/Data/Demo/demo_route.xml";
    nabiCoreSettings.m_DataSettings.m_NabiCoreParseDocuments = nabi::DataSettings::NabiCoreParseMode::All;

    nabi::NabiMainParams const nabiParams =
    {
        nabiCoreSettings
    };
    
    nabi::WinMainParams const winParams =
    {
        hInstance, hPrevInstance, lpCmdLine, nShowCmd
    };

    int const result = nabi::Main<core::Demo>(nabiParams, winParams);
    return result;
}
