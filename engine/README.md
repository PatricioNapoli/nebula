# Nebula Engine

## ToDo

### MISC
* Rework ECS
* Use include-what-you-use.
* Create a block allocator for the STL containers
* Small containers should use different allocator than the rest, which should use block
* TupleResult/Result struct with error + result T, reports to central error reporting when error is not empty
* Analytics component with metrics for vertices, materials, entities, components, etc
* Remove EngineException, replace with TupleResults, handle accordingly


### GRAPHICS
* LERP'd actions using "Layer" files with update() in them, called once every frame, can be attached to entity.
* Remake graphics pipelines with 2D and 3D different, RenderQueue has vector of RenderCommands and sorts it.
* Each Material has its shader?
* Use Uniform Buffer Objects.
* ShaderComponent (Shader graph? Includes? Common functions?) + LightComponent
* OpenGl wrappers of buffers, etc
* Extract transform from rendercomponent
* Remake rendering pipeline, RenderCommand? Make it valid across OpenGL and Vulkan.
* Remake materials rendering
* Add openGl Render struct to have render objects with VAO, EBO, etc.
* Use OpenGL types
* Unload component
* Load Material (w/ texture paths of normal,roughness,ao,metallic,albedo)
* Define common objects (cube, sphere)
* RenderBuffer, Dual Context openGl + Multithreaded rendering
* ImGui + ImGuiGizmos Editor Features, highlighting, grid
* Shader frag and vert templating
* Improve camera configuration, multiple cameras, orthographic Camera, occlusion culling
* Instancing (clone/copy object)
* Entity Groups for Batch Rendering
* Particles, Particle Emitter with billboard quads, and cubes for 3D particle
* SDF Font Rendering w/ distant field
* Layers for 2D UI Z order?
* Sorting layers and Ordering within Layers
* FrameBuffers for PostFx
* Different default shaders as to draw wireframe, lit, unlit
* 2D Text, 2D Renderer
* 3D Renderer (Forward Renderer + Deferred Lighting + PBR)
* Decals? (Quads with texture)
* Shadows
* Quaternions for 3D rotations
* Light&Shadow baking builds, lighting probes, reflection probes


### DEPENDENCIES
* https://github.com/Reputeless/Xoshiro-cpp
* Bullet, Crypto/Openssl for cert pinning, Xoshiro256+ and PCG, Network (asio?), Flatbuffers
* xor string obfuscation, OpenAl, freetype2, Volk, glsl optimizer, UI toolkit, AI & Pathfinding
* LZ4 for compressing assets for distribution (very fast decompression to memory and good ratios, 2.5x)
* Brotli for compression of network packets


### SERIALIZATION
* Objects can be serialized to Flatbuffer/flexbuffer or Nebula Object (NOBJ)
* Create stateful services/components saving data serialized to disk
* Serialization of maps
* Pointers in serializables are serializer's responsibility to serialize to string id, then it's the one that saved
    the pointer itself (and thus the persistence object), responsibility to restore it.
* Persistence class should load all objects on load and report to Register<>'d classes, the restored types.
* Game State (serialization to flatbuffers, ready for network/persistence)


### ECS
* Each unique entity type is stored as a separate Registry, in which each of the entities have their own set of components
    an entity is simply an identifier (struct concrete type) for the same Registry, they could all have different sets of components, 
    ordered in a sparse set (packed array), you can organize different entities with multiple components, in a Group.
* Components should be pools?
* ECS Registry shold allow fetch of components from different types of entities (multi-type-entity multi-component fetch)
* Listeners/observers for component addition, removal or update (instance swap)
* Represent hierarchies
* Snapshot (serialization) support and then loading for entity + components loading
* Views can be cached (could be unnecessary)
* Systems should be able to shortcircuit, to cancel a processing of an iteration of components
* Systems are registered, have an update() method in the loop, they can query all entities and filter them, WithAll, WithAny,
    WithNone, have a Lambda which can query the selected Components.


### VULKAN
* Check Awesome Vulkan github
* Use https://github.com/KhronosGroup/MoltenVK/ , Start with opengl and allow switch
* GLDebug, Vulkan validation layers


### METRICS & DEBUG
* Add more debug logging
* Add instrumentation UDP to some real time application showing metrics along with below
* Engine Metrics with memory, vertices, materials, entities, and internal system's times (Physics, AI, etc)
* Debug drawing (wireframe, framebuffer texture, depths, etc)


### MULTITHREADING
* Make use of threads for parallel shader loading, texture loading, file loading.
* MapReduce Parallel ThreadPool briefings.
* Shader Multithreaded Compiling.


### EDITOR
* Golang: Fyne - Ask in their slack
* Node.js + Skia
* ImGUI
* Go-astilectron
* CEF: https://github.com/iboB/html5-gui-demo/tree/master/demo-3d/cef
* Javascript: Node.JS Bindings (and swig?) with Electron frontend


### TECH DEBT, MAINTENANCE
* Use configs for engine and game-related configs
* Clean up public interface, hide public members or create read only accessors
* Error handling
* Code comment, wiki
* Document system requirements, Diagrams, dependencies, logo, libraries used
* Doxygen


### PROJECT BUILD
* Tiny gLTF Loader
* "Builder" to "Compile and ship current project" for game development
* Build for Linux, Mac, Windows


### OPTIMIZATION
* LOD
* Occlusion Culling


### AI
* Terrain Engine, Pathfinding
* Dynamic weather, Dynamic world changes


### SCRIPTING
* Golang with FFI bindings


### NETWORKING
* Boost.Asio / evpp
* gRPC for general Web Server connections and custom raw TCP/UDP for game states


### BIG FEATURES
* Sound OpenAL, Physics Bullet (later Havok), PostFX, Animations, Video play
* Volumetric Clouds&Fog, Hair Dynamics, Cloth Dynamics, Fluid dynamics, Weather
* Video Playing
* Sequencer/Animator
* Interface builder (grid, etc)
* Controller support
* Analytics
* Micro Transactions handled through web
* Discord Integrations
* Twitch Integrations
* Steam Integrations
* Cloud builds
* Server to connect and fetch License, login, global Asset Store
* Anti-hack through runtime integrity checks, asset files checksums, literal string obfuscation, server validation,
    anti debugger


### WATER HELP
* Perlin noise used fractally, the animation given by rotating the generating vectors. 
* The waves are Gerstner waves put on top. The foam is simulated by doing some screen space 
* raytracing to compute if a wave will intersect some geometry, and if yes I add foam + some perlin noise on top. 
* The material is just some 0.1 roughness, 0 metalness blue thing, with opacity given by looking into 
* the depthbuffer.


#### Reminder on argument semantics:
* Pass by const reference = caller is giving callee a simple copy
* Pass by reference + std::move = caller is giving callee ownership of an object and wants the original resource invalidated
* Pass by reference = caller is giving callee an efficient reference to read and write
* Pass by const reference = caller is giving callee an efficient reference to read
* Pass by pointer = caller might or might not be owner of resource, but callee has no ownership thus no responsibility to free, read and write. 
* Single exception is pointers that go outside current thread scope and have limited lifetime, those are responsibility of the final user to free.
