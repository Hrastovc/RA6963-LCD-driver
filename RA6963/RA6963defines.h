/* Check command execution capability */
#define STA0 (1 << 0)
/* Check data read/write capability */
#define STA1 (1 << 1)
/* Check Auto mode data read capability */
#define STA2 (1 << 2)
/* Check Auto mode data write capability */
#define STA3 (1 << 3)
/* Check controller operation capability */
#define STA5 (1 << 5)
/* Error flag. Used for Screen copy commands. */
#define STA6 (1 << 6)
/* Check the blink condition */
#define STA7 (1 << 7)

/* RA6963 commands */
#define RegistersSetting    (0x20)
#define CursorPointer       (0x01)
#define OffsetRegister      (0x02)
#define AddressPointer      (0x04)

#define SetControlWord      (0x40)
#define TextHomeAddress     (0x00)
#define TextArea            (0x01)
#define GraphicHomeAddress  (0x02)
#define GraphicArea         (0x03)

#define ModeSet             (0x80)
#define ORmode              (0x00)
#define EXORmode            (0x01)
#define ANDmode             (0x03)
#define TextAttributeMode   (0x04)
#define ExternalCharGenMode (0x08)
#define NormalDisplay       (0x00)
#define ReverseDisplay      (0x05)
#define InhibitDisplay      (0x03)
#define BlinkNormalDisplay  (0x08)
#define BlinkReverseDisplay (0x0D)
#define BlinkInhibitDisplay (0x0B)
#define BoldDisplay         (0x07)
#define BlinkBoldDisplay    (0x0F)

#define DisplayMode         (0x90)
#define DisplayOff          (0x00)
#define CursorBlinkOff      (0x00)
#define CursorBlinkOn       (0x01)
#define CursorOff           (0x00)
#define CursorOn            (0x02)
#define TextOff             (0x00)
#define TextOn              (0x04)
#define GraphicOff          (0x00)
#define GraphicOn           (0x08)

#define CursorPatternSelect (0xA0)
#define Cursor1line         (0x00)
#define Cursor2line         (0x01)
#define Cursor3line         (0x02)
#define Cursor4line         (0x03)
#define Cursor5line         (0x04)
#define Cursor6line         (0x05)
#define Cursor7line         (0x06)
#define Cursor8line         (0x07)

#define DataAutoReadWrite   (0xB0)
#define DataAutoWrite       (0x00)
#define DataAutoRead        (0x01)
#define AutoReset           (0x02)

#define DataReadWrite       (0xC0)
#define WriteIncrementADP   (0x00)
#define ReadIncrementADP    (0x01)
#define WriteDecrementADP   (0x02)
#define ReadDecrementADP    (0x03)
#define WriteNonvariableADP (0x04)
#define ReadNonvariableADP  (0x05)

#define ScreenPeek          (0xE0)

#define ScreenCopy          (0xE8)

#define BitSetReset         (0xF0)
#define BitReset            (0x00)
#define BitSet              (0x08)
#define Bit0                (0x00)
#define Bit1                (0x01)
#define Bit2                (0x02)
#define Bit3                (0x03)
#define Bit4                (0x04)
#define Bit5                (0x05)
#define Bit6                (0x06)
#define Bit7                (0x07)

#define ScreenReverse       (0xD0)
#define NormallyDisplay     (0x00)
#define ReverseScreen       (0x01)

#define BlinkTime66ms       (0x50)
#define BlinkTime250ms      (0x51)
#define BlinkTime500ms      (0x52)
#define BlinkTime750ms      (0x53)
#define BlinkTime1000ms     (0x54)
#define BlinkTime1250ms     (0x55)
#define BlinkTime1500ms     (0x56)
#define BlinkTime2000ms     (0x57)

#define CursorAutoMoving    (0x60)
#define CorsorAutoDisable   (0x00)
#define CorsorAutoEnable    (0x01)

#define CGROMFontSelect     (0x70)
#define CGROMFont01         (0x02)
#define CGROMFont02         (0x03)
