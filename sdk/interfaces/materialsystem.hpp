#pragma once

class IMaterialVar;
class KeyValues;
typedef UINT64 VertexFormat_t;
typedef unsigned int MorphFormat_t;

enum MaterialPropertyTypes_t
{
	MATERIAL_PROPERTY_NEEDS_LIGHTMAP = 0,					// bool
	MATERIAL_PROPERTY_OPACITY,								// int (enum MaterialPropertyOpacityTypes_t)
	MATERIAL_PROPERTY_REFLECTIVITY,							// vec3_t
	MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS				// bool
};

enum PreviewImageRetVal_t
{
	MATERIAL_PREVIEW_IMAGE_BAD = 0,
	MATERIAL_PREVIEW_IMAGE_OK,
	MATERIAL_NO_PREVIEW_IMAGE,
};

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,			// Single-channel 32-bit floating point
	IMAGE_FORMAT_RGB323232F,
	IMAGE_FORMAT_RGBA32323232F,

	// Depth-stencil texture formats for shadow depth mapping
	IMAGE_FORMAT_NV_DST16,		// 
	IMAGE_FORMAT_NV_DST24,		//
	IMAGE_FORMAT_NV_INTZ,		// Vendor-specific depth-stencil texture
	IMAGE_FORMAT_NV_RAWZ,		// formats for shadow depth mapping 
	IMAGE_FORMAT_ATI_DST16,		// 
	IMAGE_FORMAT_ATI_DST24,		//
	IMAGE_FORMAT_NV_NULL,		// Dummy format which takes no video memory

	// Compressed normal map formats
	IMAGE_FORMAT_ATI2N,			// One-surface ATI2N / DXN format
	IMAGE_FORMAT_ATI1N,			// Two-surface ATI1N format

	NUM_IMAGE_FORMATS
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = (1 << 29),
	MATERIAL_VAR_IGNORE_ALPHA_MODULATION = (1 << 30),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in imaterialinternal.h
};

enum LightType_t
{
	MATERIAL_LIGHT_DISABLE = 0,
	MATERIAL_LIGHT_POINT,
	MATERIAL_LIGHT_DIRECTIONAL,
	MATERIAL_LIGHT_SPOT,
};

struct LightDesc_t
{
	LightType_t		m_Type;
	Vector			m_Color;
	Vector	m_Position;
	Vector  m_Direction;
	float   m_Range;
	float   m_Falloff;
	float   m_Attenuation0;
	float   m_Attenuation1;
	float   m_Attenuation2;
	float   m_Theta;
	float   m_Phi;
	// These aren't used by DX8. . used for software lighting.
	float	m_ThetaDot;
	float	m_PhiDot;
	unsigned int	m_Flags;


	LightDesc_t() {}

private:
	// No copy constructors allowed
	LightDesc_t(const LightDesc_t& vOther);
};

class IMaterial
{
public:

	// Get the name of the material.  This is a full path to 
		// the vmt file starting from "hl2/materials" (or equivalent) without
		// a file extension.
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual PreviewImageRetVal_t GetPreviewImageProperties(int* width, int* height,
		ImageFormat* imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual PreviewImageRetVal_t GetPreviewImage(unsigned char* data,
		int width, int height,
		ImageFormat imageFormat) const = 0;
	// 
	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;

	virtual int				GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float* pOffset) = 0;
	virtual void			GetMaterialScale(float* pScale) = 0;
	virtual IMaterial* GetMaterialPage(void) = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to 
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;

	inline void AddRef() { IncrementReferenceCount(); }
	inline void Release() { DecrementReferenceCount(); }

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int 			GetEnumerationID(void) const = 0;

	virtual void			GetLowResColorSample(float s, float t, float* color) const = 0;

	// This computes the state snapshots for this material
	virtual void			RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool			IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool			IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool			IsVertexLit() = 0;

	// Gets the vertex format
	virtual VertexFormat_t	GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool			HasProxy(void) const = 0;

	virtual bool			UsesEnvCubemap(void) = 0;

	virtual bool			NeedsTangentSpace(void) = 0;

	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool			NeedsSoftwareSkinning(void) = 0;

	// Apply constant color or alpha modulation
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void			SetMaterialVarFlag(MaterialVarFlags_t flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(MaterialVarFlags_t flag) const = 0;

	// Gets material reflectivity 
	virtual void			GetReflectivity(Vector& reflect) = 0;

	// Gets material property flags
	virtual bool			GetPropertyFlag(MaterialPropertyTypes_t type) = 0;

	// Is the material visible from both sides?
	virtual bool			IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void			SetShader(const char* pShaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetNumPasses(void) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int				GetTextureMemoryBytes(void) = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void			Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool			NeedsSoftwareLighting(void) = 0;

	// Gets at the shader parameters
	virtual int				ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams(void) = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool			IsErrorMaterial() const = 0;

	virtual void			SetUseFixedFunctionBakedLighting(bool bEnable) = 0;

	// Gets the current alpha modulation
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float* r, float* g, float* b) = 0;

	// Gets the morph format
	virtual MorphFormat_t	GetMorphFormat() const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void			SetShaderAndParams(KeyValues* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;

	virtual void			DeleteIfUnreferenced() = 0;

	virtual bool			IsSpriteCard() = 0;

	virtual void			CallBindProxy(void* proxyData) = 0;

	virtual IMaterial* CheckProxyReplacement(void* proxyData) = 0;

	virtual void			RefreshPreservingMaterialVars() = 0;

	virtual bool			WasReloadedFromWhitelist() = 0;

	virtual bool	IsPrecached() const = 0;
};

typedef void* (*CreateInterfaceFn)(const char* Name, int* ReturnCode);

enum InitReturnVal_t
{
	INIT_FAILED = 0,
	INIT_OK,

	INIT_LAST_VAL,
};

class IMaterialProxyFactory;
class IMaterialSystemHardwareConfig;
struct MaterialSystem_Config_t;
typedef void(*MaterialBufferReleaseFunc_t)();
typedef unsigned short MaterialHandle_t;
class IShader;
class ITexture;
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
DECLARE_POINTER_HANDLE(MaterialLock_t);

enum MaterialThreadMode_t
{
	MATERIAL_SINGLE_THREADED,
	MATERIAL_QUEUED_SINGLE_THREADED,
	MATERIAL_QUEUED_THREADED
};
enum
{
	MATERIAL_ADAPTER_NAME_LENGTH = 512
};

struct MaterialAdapterInfo_t
{
	char m_pDriverName[MATERIAL_ADAPTER_NAME_LENGTH];
	unsigned int m_VendorID;
	unsigned int m_DeviceID;
	unsigned int m_SubSysID;
	unsigned int m_Revision;
	int m_nDXSupportLevel;			// This is the *preferred* dx support level
	int m_nMaxDXSupportLevel;
	unsigned int m_nDriverVersionHigh;
	unsigned int m_nDriverVersionLow;
};

struct MaterialVideoMode_t
{
	int m_Width;			// if width and height are 0 and you select 
	int m_Height;			// windowed mode, it'll use the window size
	ImageFormat m_Format;	// use ImageFormats (ignored for windowed mode)
	int m_RefreshRate;		// 0 == default (ignored for windowed mode)
};

typedef void(*ModeChangeCallbackFunc_t)(void);
struct MaterialSystemHardwareIdentifier_t;
typedef void(*MaterialBufferRestoreFunc_t)(int nChangeFlags);	// see RestoreChangeFlags_t
class IFileList;

struct MaterialSystem_SortInfo_t
{
	IMaterial* material;
	int			lightmapPageID;
};

enum MaterialContextType_t
{
	MATERIAL_HARDWARE_CONTEXT,
	MATERIAL_QUEUED_CONTEXT,
	MATERIAL_NULL_CONTEXT
};

enum HDRType_t
{
	HDR_TYPE_NONE,
	HDR_TYPE_INTEGER,
	HDR_TYPE_FLOAT,
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,			// Only allowed for render targets that don't want a depth buffer
	// (because if they have a depth buffer, the render target must be less than or equal to the size of the framebuffer).
	RT_SIZE_DEFAULT = 1,				// Don't play with the specified width and height other than making sure it fits in the framebuffer.
	RT_SIZE_PICMIP = 2,				// Apply picmip to the render target's width and height.
	RT_SIZE_HDR = 3,					// frame_buffer_width / 4
	RT_SIZE_FULL_FRAME_BUFFER = 4,	// Same size as frame buffer, or next lower power of 2 if we can't do that.
	RT_SIZE_OFFSCREEN = 5,			// Target of specified size, don't mess with dimensions
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6, // Same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	RT_SIZE_REPLAY_SCREENSHOT = 7,	// Rounded down to power of 2, essentially...
	RT_SIZE_LITERAL = 8				// Use the size passed in. Don't clamp it to the frame buffer size. Really.
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

enum CompiledVtfFlags
{
	// flags from the *.txt config file
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,

	// These are automatically generated by vtex from the texture data.
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,

	// newer flags from the *.txt config file
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,

	TEXTUREFLAGS_UNUSED_00080000 = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,

	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,

	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,

	TEXTUREFLAGS_CLAMPU = 0x02000000,

	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,					// Useable as a vertex texture

	TEXTUREFLAGS_SSBUMP = 0x08000000,

	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,

	// Clamp to border color on all texture coordinates
	TEXTUREFLAGS_BORDER = 0x20000000,

	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000,
};

class IMatRenderContext;

class IMaterialSystem
{
public:

	// Placeholder for API revision
	virtual bool					Connect(CreateInterfaceFn factory) = 0;
	virtual void					Disconnect() = 0;
	virtual void* QueryInterface(const char* pInterfaceName) = 0;
	virtual InitReturnVal_t			Init() = 0;
	virtual void					Shutdown() = 0;

	//---------------------------------------------------------
	// Initialization and shutdown
	//---------------------------------------------------------

	// Call this to initialize the material system
	// returns a method to create interfaces in the shader dll
	virtual CreateInterfaceFn		Init(char const* pShaderAPIDLL,
		IMaterialProxyFactory* pMaterialProxyFactory,
		CreateInterfaceFn fileSystemFactory,
		CreateInterfaceFn cvarFactory = NULL) = 0;

	// Call this to set an explicit shader version to use 
	// Must be called before Init().
	virtual void					SetShaderAPI(char const* pShaderAPIDLL) = 0;

	// Must be called before Init(), if you're going to call it at all...
	virtual void					SetAdapter(int nAdapter, int nFlags) = 0;

	// Call this when the mod has been set up, which may occur after init
	// At this point, the game + gamebin paths have been set up
	virtual void					ModInit() = 0;
	virtual void					ModShutdown() = 0;

	//---------------------------------------------------------
	//
	//---------------------------------------------------------

	virtual void					SetThreadMode(MaterialThreadMode_t mode, int nServiceThread = -1) = 0;
	virtual MaterialThreadMode_t	GetThreadMode() = 0;
	virtual bool					IsRenderThreadSafe() = 0;
	virtual void					ExecuteQueued() = 0;

	//---------------------------------------------------------
	// Config management
	//---------------------------------------------------------

	virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* pVersion, int* returnCode) = 0;


	// Call this before rendering each frame with the current config
	// for the material system.
	// Will do whatever is necessary to get the material system into the correct state
	// upon configuration change. .doesn't much else otherwise.
	virtual bool				UpdateConfig(bool bForceUpdate) = 0;

	// Force this to be the config; update all material system convars to match the state
	// return true if lightmaps need to be redownloaded
	virtual bool				OverrideConfig(const MaterialSystem_Config_t& config, bool bForceUpdate) = 0;

	// Get the current config for this video card (as last set by UpdateConfig)
	virtual const MaterialSystem_Config_t& GetCurrentConfigForVideoCard() const = 0;

	// Gets *recommended* configuration information associated with the display card, 
	// given a particular dx level to run under. 
	// Use dxlevel 0 to use the recommended dx level.
	// The function returns false if an invalid dxlevel was specified

	// UNDONE: To find out all convars affected by configuration, we'll need to change
	// the dxsupport.pl program to output all column headers into a single keyvalue block
	// and then we would read that in, and send it back to the client
	virtual bool				GetRecommendedConfigurationInfo(int nDXLevel, KeyValues* pKeyValues) = 0;

	// -----------------------------------------------------------
	// Device methods
	// -----------------------------------------------------------
	// Gets the number of adapters...
	virtual int					GetDisplayAdapterCount() const = 0;

	// Returns the current adapter in use
	virtual int					GetCurrentAdapter() const = 0;

	// Returns info about each adapter
	virtual void				GetDisplayAdapterInfo(int adapter, MaterialAdapterInfo_t& info) const = 0;

	// Returns the number of modes
	virtual int					GetModeCount(int adapter) const = 0;

	// Returns mode information..
	virtual void				GetModeInfo(int adapter, int mode, MaterialVideoMode_t& info) const = 0;

	virtual void				AddModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;

	// Returns the mode info for the current display device
	virtual void				GetDisplayMode(MaterialVideoMode_t& mode) const = 0;

	// Sets the mode...
	virtual bool				SetMode(void* hwnd, const MaterialSystem_Config_t& config) = 0;

	virtual bool				SupportsMSAAMode(int nMSAAMode) = 0;

	// FIXME: REMOVE! Get video card identitier
	virtual const MaterialSystemHardwareIdentifier_t& GetVideoCardIdentifier(void) const = 0;

	// Use this to spew information about the 3D layer 
	virtual void				SpewDriverInfo() const = 0;

	virtual void				GetDXLevelDefaults(UINT& max_dxlevel, UINT& recommended_dxlevel) = 0;

	// Get the image format of the back buffer. . useful when creating render targets, etc.
	virtual void				GetBackBufferDimensions(int& width, int& height) const = 0;
	virtual ImageFormat			GetBackBufferFormat() const = 0;

	virtual bool				SupportsHDRMode(HDRType_t nHDRModede) = 0;

	// -----------------------------------------------------------
	// Window methods
	// -----------------------------------------------------------

	// Creates/ destroys a child window
	virtual bool				AddView(void* hwnd) = 0;
	virtual void				RemoveView(void* hwnd) = 0;

	// Sets the view
	virtual void				SetView(void* hwnd) = 0;

	// -----------------------------------------------------------
	// Control flow
	// -----------------------------------------------------------

	virtual void				BeginFrame(float frameTime) = 0;
	virtual void				EndFrame() = 0;
	virtual void				Flush(bool flushHardware = false) = 0;

	/// FIXME: This stuff needs to be cleaned up and abstracted.
	// Stuff that gets exported to the launcher through the engine
	virtual void				SwapBuffers() = 0;

	// Flushes managed textures from the texture cacher
	virtual void				EvictManagedResources() = 0;

	virtual void				ReleaseResources(void) = 0;
	virtual void				ReacquireResources(void) = 0;

	// -----------------------------------------------------------
	// Device loss/restore
	// -----------------------------------------------------------

	// Installs a function to be called when we need to release vertex buffers + textures
	virtual void				AddReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;
	virtual void				RemoveReleaseFunc(MaterialBufferReleaseFunc_t func) = 0;

	// Installs a function to be called when we need to restore vertex buffers
	virtual void				AddRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;
	virtual void				RemoveRestoreFunc(MaterialBufferRestoreFunc_t func) = 0;

	// Release temporary HW memory...
	virtual void				ResetTempHWMemory(bool bExitingLevel = false) = 0;

	// For dealing with device lost in cases where SwapBuffers isn't called all the time (Hammer)
	virtual void				HandleDeviceLost() = 0;


	// -----------------------------------------------------------
	// Shaders
	// -----------------------------------------------------------

		// Used to iterate over all shaders for editing purposes
		// GetShaders returns the number of shaders it actually found
	virtual int					ShaderCount() const = 0;
	virtual int					GetShaders(int nFirstShader, int nMaxCount, IShader** ppShaderList) const = 0;

	// FIXME: Is there a better way of doing this?
	// Returns shader flag names for editors to be able to edit them
	virtual int					ShaderFlagCount() const = 0;
	virtual const char* ShaderFlagName(int nIndex) const = 0;

	// Gets the actual shader fallback for a particular shader
	virtual void				GetShaderFallback(const char* pShaderName, char* pFallbackShader, int nFallbackLength) = 0;


	// -----------------------------------------------------------
	// Material proxies
	// -----------------------------------------------------------

	virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;

	// Sets the material proxy factory. Calling this causes all materials to be uncached.
	virtual void				SetMaterialProxyFactory(IMaterialProxyFactory* pFactory) = 0;

	// -----------------------------------------------------------
	// Editor mode
	// -----------------------------------------------------------

	// Used to enable editor materials. Must be called before Init.
	virtual void				EnableEditorMaterials() = 0;


	// -----------------------------------------------------------
	// Stub mode mode
	// -----------------------------------------------------------

	// Force it to ignore Draw calls.
	virtual void				SetInStubMode(bool bInStubMode) = 0;


	//---------------------------------------------------------
	// Debug support
	//---------------------------------------------------------

	virtual void				DebugPrintUsedMaterials(const char* pSearchSubString, bool bVerbose) = 0;
	virtual void				DebugPrintUsedTextures(void) = 0;

	virtual void				ToggleSuppressMaterial(char const* pMaterialName) = 0;
	virtual void				ToggleDebugMaterial(char const* pMaterialName) = 0;


	//---------------------------------------------------------
	// Misc features
	//---------------------------------------------------------
	//returns whether fast clipping is being used or not - needed to be exposed for better per-object clip behavior
	virtual bool				UsingFastClipping(void) = 0;

	virtual int					StencilBufferBits(void) = 0; //number of bits per pixel in the stencil buffer


	//---------------------------------------------------------
	// Material and texture management
	//---------------------------------------------------------

	// uncache all materials. .  good for forcing reload of materials.
	virtual void				UncacheAllMaterials() = 0;

	// Remove any materials from memory that aren't in use as determined
	// by the IMaterial's reference count.
	virtual void				UncacheUnusedMaterials(bool bRecomputeStateSnapshots = false) = 0;

	// Load any materials into memory that are to be used as determined
	// by the IMaterial's reference count.
	virtual void				CacheUsedMaterials() = 0;

	// Force all textures to be reloaded from disk.
	virtual void				ReloadTextures() = 0;

	// Reloads materials
	virtual void				ReloadMaterials(const char* pSubString = NULL) = 0;

	// Create a procedural material. The keyvalues looks like a VMT file
	virtual IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues) = 0;

	// Find a material by name.
	// The name of a material is a full path to 
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	// eg. "dev/dev_bumptest" refers to somethign similar to:
	// "d:/hl2/hl2/materials/dev/dev_bumptest.vmt"
	//
	// Most of the texture groups for pTextureGroupName are listed in texture_group_names.h.
	// 
	// Note: if the material can't be found, this returns a checkerboard material. You can 
	// find out if you have that material by calling IMaterial::IsErrorMaterial().
	// (Or use the global IsErrorMaterial function, which checks if it's null too).
	virtual IMaterial* FindMaterial(char const* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = NULL) = 0;

	// Query whether a material is loaded (eg, whether FindMaterial will be nonblocking)
	virtual bool				IsMaterialLoaded(char const* pMaterialName) = 0;

	//---------------------------------
	// This is the interface for knowing what materials are available
	// is to use the following functions to get a list of materials.  The
	// material names will have the full path to the material, and that is the 
	// only way that the directory structure of the materials will be seen through this
	// interface.
	// NOTE:  This is mostly for worldcraft to get a list of materials to put
	// in the "texture" browser.in Worldcraft
	virtual MaterialHandle_t	FirstMaterial() const = 0;

	// returns InvalidMaterial if there isn't another material.
	// WARNING: you must call GetNextMaterial until it returns NULL, 
	// otherwise there will be a memory leak.
	virtual MaterialHandle_t	NextMaterial(MaterialHandle_t h) const = 0;

	// This is the invalid material
	virtual MaterialHandle_t	InvalidMaterial() const = 0;

	// Returns a particular material
	virtual IMaterial* GetMaterial(MaterialHandle_t h) const = 0;

	// Get the total number of materials in the system.  These aren't just the used
	// materials, but the complete collection.
	virtual int					GetNumMaterials() const = 0;

	//---------------------------------

	virtual void				SetAsyncTextureLoadCache(void* hFileCache) = 0;

	virtual ITexture* FindTexture(char const* pTextureName, const char* pTextureGroupName, bool complain = true, int nAdditionalCreationFlags = 0) = 0;

	// Checks to see if a particular texture is loaded
	virtual bool				IsTextureLoaded(char const* pTextureName) const = 0;

	// Creates a procedural texture
	virtual ITexture* CreateProceduralTexture(const char* pTextureName,
		const char* pTextureGroupName,
		int w,
		int h,
		ImageFormat fmt,
		int nFlags) = 0;

	//
	// Render targets
	//
	virtual void				BeginRenderTargetAllocation() = 0;
	virtual void				EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources

	// Creates a render target
	// If depth == true, a depth buffer is also allocated. If not, then
	// the screen's depth buffer is used.
	// Creates a texture for use as a render target
	virtual ITexture* CreateRenderTargetTexture(int w,
		int h,
		RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
		ImageFormat	format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED) = 0;

	virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName,				// Pass in NULL here for an unnamed render target.
		int w,
		int h,
		RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
		ImageFormat format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
		unsigned int renderTargetFlags = 0) = 0;

	virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName,
		int w,
		int h,
		RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
		ImageFormat format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		bool bClampTexCoords = true,
		bool bAutoMipMap = false) = 0;

	// Must be called between the above Begin-End calls!
	virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName,				// Pass in NULL here for an unnamed render target.
		int w,
		int h,
		RenderTargetSizeMode_t sizeMode,	// Controls how size is generated (and regenerated on video mode change).
		ImageFormat format,
		MaterialRenderTargetDepth_t depth = MATERIAL_RT_DEPTH_SHARED,
		unsigned int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT,
		unsigned int renderTargetFlags = 0) = 0;

	// -----------------------------------------------------------
	// Lightmaps
	// -----------------------------------------------------------

	// To allocate lightmaps, sort the whole world by material twice.
	// The first time through, call AllocateLightmap for every surface.
	// that has a lightmap.
	// The second time through, call AllocateWhiteLightmap for every 
	// surface that expects to use shaders that expect lightmaps.
	virtual void				BeginLightmapAllocation() = 0;
	virtual void				EndLightmapAllocation() = 0;

	// returns the sorting id for this surface
	virtual int 				AllocateLightmap(int width, int height,
		int offsetIntoLightmapPage[2],
		IMaterial* pMaterial) = 0;
	// returns the sorting id for this surface
	virtual int					AllocateWhiteLightmap(IMaterial* pMaterial) = 0;

	// lightmaps are in linear color space
	// lightmapPageID is returned by GetLightmapPageIDForSortID
	// lightmapSize and offsetIntoLightmapPage are returned by AllocateLightmap.
	// You should never call UpdateLightmap for a lightmap allocated through
	// AllocateWhiteLightmap.
	virtual void				UpdateLightmap(int lightmapPageID, int lightmapSize[2],
		int offsetIntoLightmapPage[2],
		float* pFloatImage, float* pFloatImageBump1,
		float* pFloatImageBump2, float* pFloatImageBump3) = 0;

	// fixme: could just be an array of ints for lightmapPageIDs since the material
	// for a surface is already known.
	virtual int					GetNumSortIDs() = 0;
	virtual void				GetSortInfo(MaterialSystem_SortInfo_t* sortInfoArray) = 0;

	// Read the page size of an existing lightmap by sort id (returned from AllocateLightmap())
	virtual void				GetLightmapPageSize(int lightmap, int* width, int* height) const = 0;

	virtual void				ResetMaterialLightmapPageInfo() = 0;



	virtual void	ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	// -----------------------------------------------------------
	// Access the render contexts
	// -----------------------------------------------------------
	virtual IMatRenderContext* GetRenderContext() = 0;

	virtual bool				SupportsShadowDepthTextures(void) = 0;
	virtual void				BeginUpdateLightmaps(void) = 0;
	virtual void				EndUpdateLightmaps(void) = 0;

	// -----------------------------------------------------------
	// Methods to force the material system into non-threaded, non-queued mode
	// -----------------------------------------------------------
	virtual MaterialLock_t		Lock() = 0;
	virtual void				Unlock(MaterialLock_t) = 0;

	// Vendor-dependent shadow depth texture format
	virtual ImageFormat			GetShadowDepthTextureFormat() = 0;

	virtual bool				SupportsFetch4(void) = 0;

	// Create a custom render context. Cannot be used to create MATERIAL_HARDWARE_CONTEXT
	virtual IMatRenderContext* CreateRenderContext(MaterialContextType_t type) = 0;

	// Set a specified render context to be the global context for the thread. Returns the prior context.
	virtual IMatRenderContext* SetRenderContext(IMatRenderContext*) = 0;

	virtual bool				SupportsCSAAMode(int nNumSamples, int nQualityLevel) = 0;

	virtual void				RemoveModeChangeCallBack(ModeChangeCallbackFunc_t func) = 0;

	// Finds or create a procedural material.
	virtual IMaterial* FindProceduralMaterial(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues) = 0;

	virtual ImageFormat			GetNullTextureFormat() = 0;

	virtual void				AddTextureAlias(const char* pAlias, const char* pRealName) = 0;
	virtual void				RemoveTextureAlias(const char* pAlias) = 0;

	// returns a lightmap page ID for this allocation, -1 if none available
	// frameID is a number that should be changed every frame to prevent locking any textures that are
	// being used to draw in the previous frame
	virtual int					AllocateDynamicLightmap(int lightmapSize[2], int* pOutOffsetIntoPage, int frameID) = 0;

	virtual void				SetExcludedTextures(const char* pScriptName) = 0;
	virtual void				UpdateExcludedTextures(void) = 0;

	virtual bool				IsInFrame() const = 0;

	virtual void				CompactMemory() = 0;

	// For sv_pure mode. The filesystem figures out which files the client needs to reload to be "pure" ala the server's preferences.
	virtual void ReloadFilesInList(IFileList* pFilesToReload) = 0;
	virtual	bool				AllowThreading(bool bAllow, int nServiceThread) = 0;

	// Extended version of FindMaterial().
	// Contains context in so it can make decisions (i.e. if it's a model, ignore certain cheat parameters)
	virtual IMaterial* FindMaterialEx(char const* pMaterialName, const char* pTextureGroupName, int nContext, bool complain = true, const char* pComplainPrefix = NULL) = 0;

	virtual void				DoStartupShaderPreloading(void) = 0;
};