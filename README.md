# Cesium Flight Simulator

A flight simulator in Cesium to help teach Iventis Unreal Engine 5 & Cesium!

This repo is the intended end product of the tutorial I will give. This can be used as reference during your development. The tutorial should cover:

- Using the Unreal Engine
- Creating a flight simulator using our own plane physics model
- Building the Cesium plugin from source
- Using Cesium inside Unreal Engine

## Setup

### Prerequisites
- Unreal Engine 5.1.0 or later
- Git LFS

### General Instructions

This project uses the [Cesium For Unreal](https://github.com/CesiumGS/cesium-unreal) project. It is included inside this repository as a git submodule. Instead of using Cesium's plugin binaries, this project requires the plugin to be built from source. This is so we can easily build the project on different platforms. To pull the submodule run `git submodule update --init --recursive`. This will ensure that Cesium and all of Cesium's submodules will be cloned into your directory.

To build Cesium you should follow the [Development Setup Guide](https://github.com/CesiumGS/cesium-unreal/blob/ue4-main/Documentation/developer-setup.md) and choose your platform's specific instructions. Once the plugin is built, you will need to change the `CesiumForUnreal.uplugin` file inside `Plugins/cesium-unreal`. Inside you should find a line that reads: `"EngineVersion": "5.0.0"`, this needs to be changed to `"EngineVersion": "5.1.0"`. This is so when packaging the project, Unreal Engine will package the plugin's content as well as its made for the right version.

With those changes, you now need to build the project from source. This can be done by generating visual studio project files from `CesiumFlightSim.uproject` and building the project inside Visual Studio (or whatever your choice of IDE).