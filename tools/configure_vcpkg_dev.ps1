#配置开发环境脚本
#在Windows平台用管理权限执行此脚本
#VcpkgDirRoot 为你的vcpkg的安装目录

#如果系统禁止运行脚本, 在powershell控制台中执行以下命令
#  set-ExecutionPolicy RemoteSigned


[string]$VcpkgInstalledDir= "E:/zxb/env/vcpkg/installed/"
[string]$VcpkgBuildPlatform= "x64-windows"

[string]$VcpkgIncludeDir = $VcpkgInstalledDir + $VcpkgBuildPlatform + "/include"
[string]$VcpkgLibDir = $VcpkgInstalledDir + $VcpkgBuildPlatform + "/lib"
[string]$VcpkgDebugLibDir = $VcpkgInstalledDir + $VcpkgBuildPlatform + "/debug/lib"

function ConfigureVcpkgEnv{
    [Environment]::SetEnvironmentVariable("VcpkgIncludeDir", $VcpkgIncludeDir, "Machine")
    [Environment]::SetEnvironmentVariable("VcpkgLibDir", $VcpkgLibDir, "Machine")
    [Environment]::SetEnvironmentVariable("VcpkgDebugLibDir", $VcpkgDebugLibDir, "Machine")
}

ConfigureVcpkgEnv


