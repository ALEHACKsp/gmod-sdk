#pragma once
#include "../../includes.hpp"
#include "../math/vector.hpp"

#define IN_ATTACK		(1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)
#define IN_SPEED		(1 << 17)
#define IN_WALK			(1 << 18)
#define IN_ZOOM			(1 << 19)
#define IN_WEAPON1		(1 << 20)
#define IN_WEAPON2		(1 << 21)
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)
#define IN_GRENADE2		(1 << 24)
#define IN_LOOKSPIN		(1 << 25)

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

#define NUM_BYTES 256
#define LittleLong( val )     ( val )
#define CRC32_XOR_VALUE  0xFFFFFFFFUL

typedef unsigned long CRC32_t;
#define CRC32_INIT_VALUE 0xFFFFFFFFUL

class bf_read;
class bf_write;

extern void CRC32_Init(CRC32_t* pulCRC);
extern void CRC32_Final(CRC32_t* pulCRC);

extern void CRC32_ProcessBuffer(CRC32_t* pulCRC, const void* pBuffer, int nBuffer);

struct CUserCmd
{
	std::int32_t command_nr;
	std::int32_t tick_count;
	Angle viewangles;
	float forwardmove;
	float sidemove;
	float up_move;
	std::int32_t buttons;
	std::uint8_t impulse;
	std::size_t  weapon;
	std::int32_t weapon_type;
	std::int32_t random_seed;
	std::int16_t mouse_dx;
	std::int16_t mouse_dy;
	bool         has_been_predicted;

private:
	std::uint8_t __pad[0x108]; // WARNING: Removing this will lead to dinosaurs with
							   // laser guns spawning behind you

public:
	CRC32_t GetChecksum() const
	{
		CRC32_t crc{};

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_nr, sizeof(command_nr));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &up_move, sizeof(up_move));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weapon, sizeof(weapon));
		CRC32_ProcessBuffer(&crc, &weapon_type, sizeof(weapon_type));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mouse_dx, sizeof(mouse_dx));
		CRC32_ProcessBuffer(&crc, &mouse_dy, sizeof(mouse_dy));

		CRC32_ProcessBuffer(
			&crc, __pad + 5, sizeof(bool)); // I haven't checked out what this is

		CRC32_Final(&crc);

		return crc;
	}
};

class CVerifiedUserCmd
{
public:
	CUserCmd  m_cmd;
	CRC32_t   m_crc;
};

class CWeapon;
class CameraThirdData_t;

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

struct kbutton_t
{
	// key nums holding it down
	int		down[2];
	// low bit is down state
	int		state;
};

class CInput
{
	virtual		void		Init_All(void) = 0;
	virtual		void		Shutdown_All(void) = 0;
	virtual		int			GetButtonBits(int) = 0;
	virtual		void		CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual		void		ExtraMouseSample(float frametime, bool active) = 0;
	virtual		bool		WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual		void		EncodeUserCmdToBuffer(bf_write& buf, int slot) = 0;
	virtual		void		DecodeUserCmdFromBuffer(bf_read& buf, int slot) = 0;
	virtual		CUserCmd* GetUserCmd(int sequence_number) = 0;
	virtual		void		MakeWeaponSelection(CWeapon* weapon) = 0;
	virtual		float		KeyState(kbutton_t* key) = 0;
	virtual		int			KeyEvent(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual		kbutton_t* FindKey(const char* name) = 0;
	virtual		void		ControllerCommands(void) = 0;
	virtual		void		Joystick_Advanced(void) = 0;
	virtual		void		Joystick_SetSampleTime(float frametime) = 0;
	virtual		void		IN_SetSampleTime(float frametime) = 0;
	virtual		void		AccumulateMouse(void) = 0;
	virtual		void		ActivateMouse(void) = 0;
	virtual		void		DeactivateMouse(void) = 0;
	virtual		void		ClearStates(void) = 0;
	virtual		float		GetLookSpring(void) = 0;

	virtual		void		GetFullscreenMousePos(int* mx, int* my, int* unclampedx = NULL, int* unclampedy = NULL) = 0;
	virtual		void		SetFullscreenMousePos(int mx, int my) = 0;
	virtual		void		ResetMouse(void) = 0;

	//	virtual		bool		IsNoClipping( void );
	virtual		float		GetLastForwardMove(void) = 0;
	virtual		float		Joystick_GetForward(void) = 0;
	virtual		float		Joystick_GetSide(void) = 0;
	virtual		float		Joystick_GetPitch(void) = 0;
	virtual		float		Joystick_GetYaw(void) = 0;

	virtual		void		CAM_Think(void) = 0;
	virtual		int			CAM_IsThirdPerson(void) = 0;
	virtual		void		CAM_ToThirdPerson(void) = 0;
	virtual		void		CAM_ToFirstPerson(void) = 0;
	virtual		void		CAM_StartMouseMove(void) = 0;
	virtual		void		CAM_EndMouseMove(void) = 0;
	virtual		void		CAM_StartDistance(void) = 0;
	virtual		void		CAM_EndDistance(void) = 0;
	virtual		int			CAM_InterceptingMouse(void) = 0;

	// orthographic camera info
	virtual		void		CAM_ToOrthographic() = 0;
	virtual		bool		CAM_IsOrthographic() = 0;
	virtual		void		CAM_OrthographicSize(float& w, float& h) const = 0;
public:
	BYTE pad__01[136]; // 0x8B & m_fCameraMovingWithMouse = true thirdperson working
	// Is the 3rd person camera using the mouse?
	bool		m_fCameraInterceptingMouse;
	// Are we in 3rd person view?
	bool		m_fCameraInThirdPerson;
	// Should we move view along with mouse?
	bool		m_fCameraMovingWithMouse;


	// Is the camera in distance moving mode?
	bool		m_fCameraDistanceMove; // 37
	// Old and current mouse position readings.
	int			m_nCameraOldX;
	int			m_nCameraOldY;
	int			m_nCameraX;
	int			m_nCameraY;

	// orthographic camera settings
	bool		m_CameraIsOrthographic; // 1

	Angle		m_angPreviousViewAngles; // 12

	float		m_flLastForwardMove; // 4

	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;
	CUserCmd* cmds;
	CVerifiedUserCmd* verified_cmds;

	CameraThirdData_t* m_pCameraThirdData;
};