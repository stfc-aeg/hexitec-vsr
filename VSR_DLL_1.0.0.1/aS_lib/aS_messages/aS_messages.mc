MessageIdTypedef=DWORD

SeverityNames=(Success=0x0:STATUS_SEVERITY_SUCCESS
    Informational=0x1:STATUS_SEVERITY_INFORMATIONAL
    Warning=0x2:STATUS_SEVERITY_WARNING
    Error=0x3:STATUS_SEVERITY_ERROR
    )


FacilityNames=(
	aSpect=0x000:FACILITY_AS
	aSpectFG=0x001:FACILITY_AS_FG
	aSpectConvert=0x002:FACILITY_AS_CONVERT
	aSpectVSR=0x003:FACILITY_AS_VSR
	System=0x100:FACILITY_SYSTEM
    NiImaq=0x200:FACILITY_NI_IMAQ
    NiSystem=0x300:FACILITY_NI_SYSTEM
	PleoraGigE=0x400:FACILITY_PLEORA_GIGE_SYSTEM
)

LanguageNames=(English=0x409:MSG00409)
LanguageNames=(German=0x407:MSG00407)

;// aSpect --------------------------------------------------------------------

MessageId=0x0
Severity=Success
Facility=aSpect
SymbolicName=AS_NO_ERROR
Language=English
No error occurred (%1!d!).
.
Language=German
Kein Fehler aufgetreten (%1!d!).
.

MessageId=0x1
Severity=Error
Facility=aSpect
SymbolicName=AS_MUTEX_NOT_LONGER_VALID
Language=English
MUTEX Object is not longer valid (%1!d!). 
.
Language=German
MUTEX Object is nicht mehr gültig (%1!d!).
.

MessageId=0x2
Severity=Error
Facility=aSpect
SymbolicName=AS_WAIT_TIMEOUT
Language=English
Timeout occurred at wait for Object (%1!d!).
.
Language=German
Zeitüberschreitung beim Warten auf ein Object aufgetreten (%1!d!).
.

MessageId=0x3
Severity=Error
Facility=aSpect
SymbolicName=AS_INVALID_MUTEX
Language=English
MUTEX Object is not valid (%1!d!). 
.
Language=German
MUTEX Object is nicht gültig (%1!d!).
.

MessageId=0x4
Severity=Error
Facility=aSpect
SymbolicName=AS_INSTANCE_BUSY
Language=English
Instance already in use (%1!d!). 
.
Language=German
Instanz bereits belegt (%1!d!).
.

MessageId=0x5
Severity=Error
Facility=aSpect
SymbolicName=AS_INSTANCE_READY
Language=English
Instance not in use (%1!d!). 
.
Language=German
Instanz nicht belegt (%1!d!).
.

MessageId=0x6
Severity=Error
Facility=aSpect
SymbolicName=AS_UNKNOWN_INSTANCE
Language=English
Unknown instance specified (%1!d!). 
.
Language=German
Unbekannte Instanz Übergeben (%1!d!).
.

MessageId=0x7
Severity=Error
Facility=aSpect
SymbolicName=AS_NULL_POINTER
Language=English
One parameter is a NULL pointer which should not be (%1!d!). 
.
Language=German
Ein Parameter ist ein NULL Pointer der keine sein sollte (%1!d!).
.

MessageId=0x8
Severity=Error
Facility=aSpect
SymbolicName=AS_FPGA_FW_CHECK_CUSTOMER_ERROR
Language=English
FPGA firmware version check failed. CustomerID does not match (%1!d!). 
.
Language=German
Die FPGA Firmware Versionsüberprüfung ist fehlgeschlagen. Die CustomerID stimmt nicht überein (%1!d!).
.

MessageId=0x9
Severity=Error
Facility=aSpect
SymbolicName=AS_FPGA_FW_CHECK_PROJECT_ERROR
Language=English
FPGA firmware version check failed. ProjectID does not match (%1!d!). 
.
Language=German
Die FPGA Firmware Versionsüberprüfung ist fehlgeschlagen. Die ProjectID stimmt nicht überein (%1!d!).
.

MessageId=0xA
Severity=Error
Facility=aSpect
SymbolicName=AS_FPGA_FW_CHECK_VERSION_ERROR
Language=English
FPGA firmware version check failed. Version does not match (%1!d!). 
.
Language=German
Die FPGA Firmware Versionsüberprüfung ist fehlgeschlagen. Die Version stimmt nicht überein (%1!d!).
.

MessageId=0xB
Severity=Error
Facility=aSpect
SymbolicName=AS_BUFFER_TO_SMALL
Language=English
Buffer to small (%1!d!). 
.
Language=German
Puffer zu klein (%1!d!).
.

;// aSpectFG ------------------------------------------------------------------
;// 0x2 free

MessageId=0x1
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_CREATE_EVENT_ERR
Language=English
Event-handle could not be created (FrameGrabberLib: %1!d!). 
.
Language=German
Event-handle konnte nicht erstellt werden (FrameGrabberLib: %1!d!).
.

MessageId=0x3
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_CREATE_SEMAPHORE_ERR
Language=English
Semaphore could not be created (FrameGrabberLib: %1!d!). 
.
Language=German
Semaphore konnte nicht erstellt werden (FrameGrabberLib: %1!d!).
.

MessageId=0x4
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_CREATE_THREAD_ERR
Language=English
Thread could not be created (FrameGrabberLib: %1!d!). 
.
Language=German
Thread konnte nicht erstellt werden (FrameGrabberLib: %1!d!).
.

MessageId=0x5
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WAIT_OBJECT_ERR
Language=English
Wait for object failed (FrameGrabberLib: %1!d!). 
.
Language=German
Warten auf Objekt fehlgeschlagen (FrameGrabberLib: %1!d!).
.

MessageId=0x6
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_INVALID_THREAD_CNT
Language=English
The parameter "threadCnt" is out of Range (FrameGrabberLib: %1!d!). 
.
Language=German
Der Parameter "threadCnt" liegt außerhalb des gültigen Bereiches (FrameGrabberLib: %1!d!).
.

MessageId=0x7
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_INVALID_INDEX
Language=English
The parameter "idx" is greater than the array size of the parameter "fgIfNames" (FrameGrabberLib: %1!d!). 
.
Language=German
Der Parameter "idx" ist größer als die Array Größe von dem Parameter "fgIfNames" (FrameGrabberLib: %1!d!).
.

MessageId=0x8
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_INVALID_FGIFNAMES_SIZE
Language=English
The array "fgIfNames" have to be greater than 0 (FrameGrabberLib: %1!d!). 
.
Language=German
Das Array "fgIfNames" muß größer als 0 sein (FrameGrabberLib: %1!d!).
.

MessageId=0x9
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_NOT_INITIALIZED
Language=English
The framegrabber instance is not initialized (FrameGrabberLib: %1!d!). 
.
Language=German
Die Frame-Grabber Instanz ist nicht initialisiert (FrameGrabberLib: %1!d!).
.

MessageId=0xA
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_SESSION_NOT_CONFIGURED
Language=English
Session is not configured (FrameGrabberLib: %1!d!). 
.
Language=German
Die Session ist nicht konfiguriert (FrameGrabberLib: %1!d!).
.

MessageId=0xB
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_DEST_IMG_PARAMETER
Language=English
Invalid properties of destination image (image size | image type)(FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Eigenschaften des Zielbildes (Bildgröße | Bildtyp)(FrameGrabberLib: %1!d!).
.

MessageId=0xC
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_CORRECT_IMG_PARAMETER
Language=English
Invalid properties of correction image (image size | image type)(FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Eigenschaften des Korrekturbildes (Bildgröße | Bildtyp)(FrameGrabberLib: %1!d!).
.

MessageId=0xD
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_EXT_TRIGGER_OUT_ERR
Language=English
External trigger out not configured (FrameGrabberLib: %1!d!). 
.
Language=German
Externe Triggerleitung nicht als Ausgang konfiguriert (FrameGrabberLib: %1!d!).
.

MessageId=0xE
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_FUNCTION_NUMBER
Language=English
Function number is out of Range (FrameGrabberLib: %1!d!). 
.
Language=German
Funktionsnummer liegt außerhalb des gültigen Bereiches (FrameGrabberLib: %1!d!).
.

MessageId=0xF
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_RETURNED_BUFFER_ERR
Language=English
Returned buffer does not match the requested buffer (FrameGrabberLib: %1!d!). 
.
Language=German
Der zurückgegebene Puffer stimmt nicht mit dem angefragten Puffer überein (FrameGrabberLib: %1!d!).
.

MessageId=0x10
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_LOST_FRAMES_OCCURED
Language=English
Lost frames occured (FrameGrabberLib: %1!d!). 
.
Language=German
Bilder in der Sequenz sind verloren (FrameGrabberLib: %1!d!).
.

MessageId=0x11
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_DIV_EQUAL_0
Language=English
Divide by zero not possible (FrameGrabberLib: %1!d!). 
.
Language=German
Division durch 0 nicht erlaubt (FrameGrabberLib: %1!d!).
.

MessageId=0x12
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_DEST_BUFFER_PARAMETER
Language=English
Buffer pointer NULL or buffer to small (FrameGrabberLib: %1!d!). 
.
Language=German
Pufferzeiger gleich NULL oder Puffer zu klein (FrameGrabberLib: %1!d!).
.

MessageId=0x13
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_MASK_LEN
Language=English
Invalid length of mask array (FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Länge des Masken Arrays (FrameGrabberLib: %1!d!).
.

MessageId=0x14
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_DARK_IMG_PARAMETER
Language=English
Invalid properties of dark correction image (image size | image type)(FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Eigenschaften des Dunkelkorrekturbildes (Bildgröße | Bildtyp)(FrameGrabberLib: %1!d!).
.

MessageId=0x15
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_IMG_CNT
Language=English
Invalid image count (FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Bildanzahl (FrameGrabberLib: %1!d!).
.

MessageId=0x16
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_ROI_PARAMETER
Language=English
Invalid properties of region of interest (ROI Start | ROI Size | Read Out Start)(FrameGrabberLib: %1!d!). 
.
Language=German
Ungültige Eigenschaften der ROI (ROI Start | ROI Größe | Read Out Start)(FrameGrabberLib: %1!d!).
.

MessageId=0x17
Severity=Error
Facility=aSpectFG
SymbolicName=AS_FG_WRONG_SENSOR_TYPE
Language=English
Invalid sensor type (FrameGrabberLib: %1!d!). 
.
Language=German
Ungültiger Sensor Type (FrameGrabberLib: %1!d!).
.

;// aSpectConvert -------------------------------------------------------------

MessageId=0x1
Severity=Error
Facility=aSpectConvert
SymbolicName=AS_CONVERT_TO_STRING_ERR
Language=English
Number to string convertion failed (%1!d!). 
.
Language=German
Nummer zu String Umwandlung fehlgeschlagen (%1!d!).
.

;// aSpectVSR -----------------------------------------------------------------

MessageId=0x01
Severity=Error
Facility=aSpectVSR
SymbolicName=AS_VSR_RESOLUTION_NOT_MATCH
Language=English
Resolution of VSR1 does not match with resolution of VSR2. Check sensor setup register settings (%1!d!).
.
Language=German
Die Auflösung vom VSR1 stimmt nicht mit der Auflösung vom VSR2 überein. Sensor Setup Register Einstellungen überprüfen (%1!d!).
.

MessageId=0x02
Severity=Error
Facility=aSpectVSR
SymbolicName=AS_VSR_CH_TRAINING_NOT_COMPLETED
Language=English
Channel training not completed. Check cable connections (%1!d!).
.
Language=German
Der Abgleich der Übertragungskanäle wurde nicht abgeschlossen. Kabelverbindungen überprüfen (%1!d!).
.

;// System --------------------------------------------------------------------

MessageId=0x1
Severity=Error
Facility=System
SymbolicName=AS_RELEASE_SEMAPHORE_ERR
Language=English
Release Semaphore failed (%1!d!). 
.
Language=German
Freigeben der Semaphore ist fehlgeschlagen (%1!d!).
.

MessageId=0x2
Severity=Error
Facility=System
SymbolicName=AS_OPENFILE_ERR
Language=English
File open failed (%1!d!). 
.
Language=German
Öffnen der Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0x3
Severity=Error
Facility=System
SymbolicName=AS_GET_FILE_SIZE_ERR
Language=English
GetFileSize failed (%1!d!). 
.
Language=German
Ermitteln der Dateigröße ist fehlgeschlagen (%1!d!).
.

MessageId=0x4
Severity=Error
Facility=System
SymbolicName=AS_READ_FILE_ERR
Language=English
ReadFile failed (%1!d!). 
.
Language=German
Lesen der Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0x5
Severity=Error
Facility=System
SymbolicName=AS_SYS_CREATE_EVENT_ERR
Language=English
Event-handle could not be created (%1!d!). 
.
Language=German
Event-handle konnte nicht erstellt werden (%1!d!).
.

MessageId=0x6
Severity=Error
Facility=System
SymbolicName=AS_SYS_RESET_EVENT_ERR
Language=English
Event-Reset failed (%1!d!). 
.
Language=German
Event-Reset fehlgeschlagen (%1!d!).
.

MessageId=0x7
Severity=Error
Facility=System
SymbolicName=AS_SYS_SET_EVENT_ERR
Language=English
Event-Set failed (%1!d!). 
.
Language=German
Event-Set fehlgeschlagen (%1!d!).
.

MessageId=0x8
Severity=Error
Facility=System
SymbolicName=AS_SYS_CREATE_DIR_ERR
Language=English
Create directory failed (%1!d!). 
.
Language=German
Verzeichniserstellung ist fehlgeschlagen (%1!d!).
.

MessageId=0x9
Severity=Error
Facility=System
SymbolicName=AS_CREATEFILE_ERR
Language=English
Create file failed (%1!d!). 
.
Language=German
Erstellen der Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0xA
Severity=Error
Facility=System
SymbolicName=AS_WRITEFILE_ERR
Language=English
Write file failed (%1!d!). 
.
Language=German
Schreiben in Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0xB
Severity=Error
Facility=System
SymbolicName=AS_CLOSEFILE_ERR
Language=English
Close file failed (%1!d!). 
.
Language=German
Schliessen der Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0xC
Severity=Error
Facility=System
SymbolicName=AS_FINDFILE_ERR
Language=English
Find file failed (%1!d!). 
.
Language=German
Suchen der Datei ist fehlgeschlagen (%1!d!).
.

;// NiImaq --------------------------------------------------------------------

MessageId=0x1
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_INTERFACE_OPEN_ERR
Language=English
Framegrabber-Interface could not be opened (%1!d!). 
.
Language=German
Framegrabber-Interface konnte nicht geöffnet werden (%1!d!).
.

MessageId=0x2
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SESSION_OPEN_ERR
Language=English
Framegrabber-Session could not be opened (%1!d!). 
.
Language=German
Framegrabber-Session konnte nicht geöffnet werden (%1!d!).
.

MessageId=0x3
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SESSION_FIT_ROI_ERR
Language=English
ROI could not be properly aligned (%1!d!). 
.
Language=German
ROI konnte nicht angepasst werden (%1!d!).
.

MessageId=0x4
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SESSION_CONFIGURE_ROI_ERR
Language=English
ROI could not be set (%1!d!). 
.
Language=German
ROI konnte nicht gesetzt werden (%1!d!).
.

MessageId=0x5
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_GET_BUFFER_SIZE_ERR
Language=English
Needed buffer size could not determined (%1!d!). 
.
Language=German
Benötigte Buffer Größe konnte nicht bestimmt werden (%1!d!).
.

MessageId=0x6
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_CREATE_BUFFER_LIST_ERR
Language=English
Buffer List could not be created (%1!d!). 
.
Language=German
Buffer Liste konnte nicht erstellt werden (%1!d!).
.

MessageId=0x7
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_CREATE_BUFFER_ERR
Language=English
Buffer could not be created (%1!d!). 
.
Language=German
Buffer konnte nicht erstellt werden (%1!d!).
.

MessageId=0x8
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SET_BUFFER_ADDRESS_ERR
Language=English
Buffer address could not be set (%1!d!). 
.
Language=German
Buffer Adresse konnte nicht erstellt werden (%1!d!).
.

MessageId=0x9
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SET_BUFFER_SIZE_ERR
Language=English
Buffer size could not be set (%1!d!). 
.
Language=German
Buffer Größe konnte nicht erstellt werden (%1!d!).
.

MessageId=0xA
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SET_BUFFER_COMMAND_ERR
Language=English
Buffer command could not be set (%1!d!). 
.
Language=German
Buffer Kommando konnte nicht erstellt werden (%1!d!).
.

MessageId=0xB
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SESSION_CONFIGURE_ERR
Language=English
Session could not be configured (%1!d!). 
.
Language=German
Session konnte nicht konfiguriert werden (%1!d!).
.

MessageId=0xC
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_DISPOSE_BUFFER_LIST_ERR
Language=English
Dispose buffer list failed (%1!d!). 
.
Language=German
Buffer Liste konnte nicht gelöscht werden (%1!d!).
.

MessageId=0xD
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_GET_ROI_ERR
Language=English
Get ROI failed (%1!d!). 
.
Language=German
ROI konnte nicht ermittelt werden (%1!d!).
.

MessageId=0xE
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_CREATE_PULSE
Language=English
Pulse creation failed (%1!d!). 
.
Language=German
Impuls konnte nicht erstellt werden (%1!d!).
.

MessageId=0xF
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_TRIGGER_CONFIGURE
Language=English
Trigger configuration failed (%1!d!). 
.
Language=German
Trigger Konfiguration ist fehlgeschlagen (%1!d!).
.

MessageId=0x10
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_TRIGGER_DRIVE_ERR
Language=English
Drive trigger failed (%1!d!). 
.
Language=German
Treiben der Triggerleitung ist fehlgeschlagen (%1!d!).
.

MessageId=0x11
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_PULSE_DISPOSE_ERR
Language=English
Dispose pulse failed (%1!d!). 
.
Language=German
Löschen des Impulses ist fehlgeschlagen (%1!d!).
.

MessageId=0x12
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SESSION_START_ACQ_ERR
Language=English
Acquisition start failed (%1!d!). 
.
Language=German
Start der Aufnahme ist fehlgeschlagen (%1!d!).
.

MessageId=0x13
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_PULSE_START_ERR
Language=English
Pulse start failed (%1!d!). 
.
Language=German
Starten des Impulses ist fehlgeschlagen (%1!d!).
.

MessageId=0x14
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_PULSE_STOP_ERR
Language=English
Pulse stop failed (%1!d!). 
.
Language=German
Stoppen des Impulses ist fehlgeschlagen (%1!d!).
.

MessageId=0x15
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_EXAMINE_BUFFER_ERR
Language=English
Examine buffer failed (%1!d!). 
.
Language=German
Ermitteln des aktuellen Puffers ist fehlgeschlagen (%1!d!).
.

MessageId=0x16
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_RELEASE_BUFFER_ERR
Language=English
Release buffer failed (%1!d!). 
.
Language=German
Freigeben des Puffers ist fehlgeschlagen (%1!d!).
.

MessageId=0x17
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_GET_ATTRIBUTE_ERR
Language=English
Get attribute failed (%1!d!). 
.
Language=German
Auslesen des Attributes ist fehlgeschlagen (%1!d!).
.

MessageId=0x18
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SERIAL_FLUSH_ERR
Language=English
SerialFlush over framegrabber error (%1!d!). 
.
Language=German
Löschen des seriellen Empfangspuffer ist fehlgeschlagen (%1!d!).
.

MessageId=0x19
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SERIAL_WRITE_ERR
Language=English
SerialWrite over framegrabber failed (%1!d!). 
.
Language=German
Serielles Schreiben über dem Framegrabber ist fehlgeschlagen (%1!d!).
.

MessageId=0x1A
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SERIAL_READ_ERR
Language=English
SerialRead over framegrabber failed (%1!d!). 
.
Language=German
Serielles Lesen über dem Framegrabber ist fehlgeschlagen (%1!d!).
.

MessageId=0x1B
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_WAIT_SIGNAL_ASYNC_ERR
Language=English
Configure wait for signal failed (%1!d!). 
.
Language=German
Konfigurieren der Funktion "Warten auf ein Signal" ist fehlgeschlagen (%1!d!).
.

MessageId=0x1C
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SET_ATTRIBUTE_ERR
Language=English
Set attribute failed (%1!d!). 
.
Language=German
Setzen des Attributes ist fehlgeschlagen (%1!d!).
.

MessageId=0x1D
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_SET_CAMERAFILE_ERR
Language=English
Change camera file failed (%1!d!). 
.
Language=German
Ändern der Camera Datei ist fehlgeschlagen (%1!d!).
.

MessageId=0x1E
Severity=Error
Facility=NiImaq
SymbolicName=AS_FG_STOP_ACQ_ERR
Language=English
Stop acquisition failed (%1!d!). 
.
Language=German
Stoppen der Bildaufnahme ist fehlgeschlagen (%1!d!).
.

;// NiSystem ------------------------------------------------------------------

MessageId=0x1
Severity=Error
Facility=NiSystem
SymbolicName=AS_NI_SYS_INVALID_MSG_BUFFER
Language=English
Message buffer for NI error Message too small (min. 256 bytes)(%1!d!). 
.
Language=German
Puffer für NI Fehlermeldung zu klein (min. 256 bytes)(%1!d!).
.

;// PleoraGigE ----------------------------------------------------------------

MessageId=0x1
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_INIT_DEVICE_ERROR
Language=English
Could not create the GigE device instance (GigE Lib: %1!d!). 
.
Language=German
GigE Geräteinstanz konnte nicht erstellt werden (GigE Lib: %1!d!).
.

MessageId=0x2
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_OPEN_ERROR
Language=English
Could not open the serial port (GigE Lib: %1!d!). 
.
Language=German
Serielle Schnittstelle konnte nicht geöffnet werden (GigE Lib: %1!d!).
.

MessageId=0x3
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_CLOSE_ERROR
Language=English
Could not close the serial port (GigE Lib: %1!d!). 
.
Language=German
Serielle Schnittstelle konnte nicht geschlossen werden (GigE Lib: %1!d!).
.

MessageId=0x4
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_NOT_SUPPORTED
Language=English
Specified serial port is not supported by the device (GigE Lib: %1!d!). 
.
Language=German
Die angegebene serielle Schnittstelle wird von dem Gerät nicht unterstützt (GigE Lib: %1!d!).
.

MessageId=0x5
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_CONFIG_FAILED
Language=English
Could not configure the serial port (GigE Lib: %1!d!). 
.
Language=German
Die serielle Schnittstelle konnte nicht konfiguriert werden (GigE Lib: %1!d!).
.

MessageId=0x6
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_SET_RX_BUFFER_FAILED
Language=English
Could not set the receive buffer size of the serial port (GigE Lib: %1!d!). 
.
Language=German
Die Größe des Empfangspuffers der seriellen Schnittstell konnte nicht gesetzt werden (GigE Lib: %1!d!).
.

MessageId=0x7
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_FLUSH_ERROR
Language=English
Could not flush the receive buffer of the serial port (GigE Lib: %1!d!). 
.
Language=German
Der Empfangspuffer der serielle Schnittstelle konnte nicht geleert werden (GigE Lib: %1!d!).
.

MessageId=0x8
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_READ_ERROR
Language=English
Read from the serial port failed (GigE Lib: %1!d!). 
.
Language=German
Das Lesen von der seriellen Schnittstelle ist fehlgeschlagen (GigE Lib: %1!d!).
.

MessageId=0x9
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_READ_BUFFER_FULL
Language=English
The receive buffer of the serial port is full without receiving the termination character (GigE Lib: %1!d!). 
.
Language=German
Der Empfangspuffer der seriellen Schnittstelle ist voll und es wurde kein Ende Zeichen empfangen (GigE Lib: %1!d!).
.

MessageId=0xA
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SERIAL_PORT_WRITE_ERROR
Language=English
Write to the serial port failed (GigE Lib: %1!d!). 
.
Language=German
Das Schreiben an die serielle Schnittstelle ist fehlgeschlagen (GigE Lib: %1!d!).
.

MessageId=0xB
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PIPELINE_CREATION_ERROR
Language=English
Could not create the pipeline (GigE Lib: %1!d!). 
.
Language=German
Die Pipeline konnte nicht erstellt werden (GigE Lib: %1!d!).
.

MessageId=0xC
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PIPELINE_NOT_AVAILABLE
Language=English
No pipeline available (GigE Lib: %1!d!). 
.
Language=German
Pipeline nicht verfügbar (GigE Lib: %1!d!).
.

MessageId=0xD
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PIPELINE_CREATED_ALREADY
Language=English
Pipeline already created (GigE Lib: %1!d!). 
.
Language=German
Pipeline ist bereits erstellt (GigE Lib: %1!d!).
.

MessageId=0xE
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PIPELINE_START_ERROR
Language=English
Failed to start the pipeline (GigE Lib: %1!d!). 
.
Language=German
Die Pipeline konnte nicht gestartet werden (GigE Lib: %1!d!).
.

MessageId=0xF
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PIPELINE_STOP_ERROR
Language=English
Failed to stop the pipeline (GigE Lib: %1!d!). 
.
Language=German
Die Pipeline konnte nicht gestoppt werden (GigE Lib: %1!d!).
.

MessageId=0x10
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_RESET_COMMAND_ERROR
Language=English
Invoking the reset command failed (GigE Lib: %1!d!). 
.
Language=German
Ausführen des Reset Kommandos ist fehlgeschlagen (GigE Lib: %1!d!).
.

MessageId=0x11
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_START_COMMAND_ERROR
Language=English
Invoking the start command failed (GigE Lib: %1!d!). 
.
Language=German
Ausführen des Start Kommandos ist fehlgeschlagen (GigE Lib: %1!d!).
.

MessageId=0x12
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STOP_COMMAND_ERROR
Language=English
Invoking the stop command failed (GigE Lib: %1!d!). 
.
Language=German
Ausführen des Stop Kommandos ist fehlgeschlagen (GigE Lib: %1!d!).
.

MessageId=0x13
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_OPEN_ERROR
Language=English
Could not open the stream (GigE Lib: %1!d!). 
.
Language=German
Der Stream konnte nicht geöffnet werden (GigE Lib: %1!d!).
.

MessageId=0x14
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_CLOSE_ERROR
Language=English
Could not close the stream (GigE Lib: %1!d!). 
.
Language=German
Der Stream klonnte nicht geschlossen werden (GigE Lib: %1!d!).
.

MessageId=0x15
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_CONFIG_ERROR
Language=English
Could not configure the stream (GigE Lib: %1!d!). 
.
Language=German
Der Stream konnte nicht konfiguriert werden (GigE Lib: %1!d!).
.

MessageId=0x16
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_NOT_AVAILABLE
Language=English
No stream available (GigE Lib: %1!d!). 
.
Language=German
Stream nicht verfügbar (GigE Lib: %1!d!).
.

MessageId=0x17
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_ALREADY_OPENED
Language=English
Stream already created (GigE Lib: %1!d!). 
.
Language=German
Stream ist bereits erstellt (GigE Lib: %1!d!).
.

MessageId=0x18
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_ENABLE_ERROR
Language=English
Could not enable the stream (GigE Lib: %1!d!). 
.
Language=German
Der Stream konnte nicht aktiviert werden (GigE Lib: %1!d!).
.

MessageId=0x19
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_STREAM_DISABLE_ERROR
Language=English
Could not disable the stream (GigE Lib: %1!d!). 
.
Language=German
Der Stream konnte nicht deaktiviert werden (GigE Lib: %1!d!).
.

MessageId=0x1A
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_BLOCKS_DROPPED
Language=English
Blocks dropped during the acquisition (GigE Lib: %1!d!). 
.
Language=German
Während der Erfassung sind Blöcke verworfen worden (GigE Lib: %1!d!).
.

MessageId=0x1B
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_BLOCKS_IDS_MISSING
Language=English
Block IDs missing during the acquisition (GigE Lib: %1!d!). 
.
Language=German
Während der Erfassung sind Block IDs verloren gegangen (GigE Lib: %1!d!).
.

MessageId=0x1C
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_ACQUISION_ABORTED_ERROR
Language=English
Acquisition aborted caused by an error (GigE Lib: %1!d!). 
.
Language=German
Die Erfassung ist Aufgrund eines Fehlers abgebrochen (GigE Lib: %1!d!).
.

MessageId=0x1D
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_GET_INTEGER_VALUE_ERROR
Language=English
Could not get the integer value (GigE Lib: %1!d!). 
.
Language=German
Die Ganzzahl konnte nicht empfangen werden (GigE Lib: %1!d!).
.

MessageId=0x1E
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_NO_TRANSFER_BUFFER_AVAILABLE
Language=English
No transfer buffer available (GigE Lib: %1!d!). 
.
Language=German
Kein Transfer Puffer verfügbar (GigE Lib: %1!d!).
.

MessageId=0x1F
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_SET_IMAGE_FORMAT_ERROR
Language=English
Could not set the image parameter (GigE Lib: %1!d!). 
.
Language=German
Die Bildeigenschaften konnten nicht gesetzt werden (GigE Lib: %1!d!).
.

MessageId=0x20
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_COLLECT_DC_NOT_READY
Language=English
Could not collect the offset correction values within the specified time (GigE Lib: %1!d!). 
.
Language=German
Die Offset Korrekturwerte konnten nicht in der angegebenen Zeit erfasst werden (GigE Lib: %1!d!).
.

MessageId=0x21
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_PLEORA_USB_DRIVER_NOT_INSTALLED
Language=English
The Pleora USB driver is for this device either not installed or not active (GigE Lib: %1!d!). 
.
Language=German
Der Pleora USB Treiber ist für dieses Gerät entweder nicht installiert oder nicht aktiv (GigE Lib: %1!d!).
.

MessageId=0x22
Severity=Error
Facility=PleoraGigE
SymbolicName=AS_GIGE_USB3_NOT_ACTIVE
Language=English
The device is not connected to a USB3 port (GigE Lib: %1!d!). 
.
Language=German
Das Gerät ist nicht mit einem USB3 Port verbunden (GigE Lib: %1!d!).
.
