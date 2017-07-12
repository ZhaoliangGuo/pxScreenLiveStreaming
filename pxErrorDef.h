#pragma once

#include <WinError.h>

//----------------------------------------------------------------
//                 NSErrors.h 
//----------------------------------------------------------------
//
// Define the Macro which will create the HRESULT
//
#define _FACNSPG   0x888 // 0x999
#define MAKE_NSHRESULTERROR(code) MAKE_HRESULT( 1, _FACNSPG, code )
#define MAKE_NSHRESULTWARNING(code) MAKE_HRESULT( 0, _FACNSPG, code )



//
// A HRESULT error code is represented like this:
// 
// ------------------------------------------------------
// | Serv | Factory Code |        Error Code            |
// ------------------------------------------------------
//
// |<----------------  32 bits  ----------------------->|
//
// Serv
//      Length: 1 nibble (4 bits)
//      Values: always 0x1 for the Error Code
//              always 0x0 for the Warning Code
//
// Factory Code
//      Length: 3 nibbles (12 Bits)
//      Value:  For the common errors of Video Product, the value is 0x999
//
// Error Code
//      Length: 1 Word (16 bits)
//      Value: In the case of an error, the MSB is 1 
//             In the case of a warning, the MSB is 0
//

#define NS_NOERROR 0 // No error           
#define NS_TRUE    0 // TRUE
#define NS_FALSE   1 // FALSE

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ------------------------ ERROR CODES ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


#define NS_E_FAIL                                              MAKE_NSHRESULTERROR (0x8FFF)  // Generic error.
#define NS_E_INVALID_STRUCTURE_SIZE                            MAKE_NSHRESULTERROR (0x8001)  // Structure size not set or not valid.
#define NS_E_NOT_SUPPORTED                                     MAKE_NSHRESULTERROR (0x8002)  // The requested operation is not supported.
#define NS_E_NOT_IMPLEMENTED                                   MAKE_NSHRESULTERROR (0x8003)  // The code is not implemented yet for the requested operation.
#define NS_E_ALREADY_ATTACHED                                  MAKE_NSHRESULTERROR (0x8004)  // A custom object with the same GUID is already attached.
#define NS_E_NOT_FOUND                                         MAKE_NSHRESULTERROR (0x8005)  // No custom object of this GUID has been attached or requested item could not be found.
#define NS_E_LIST_CORRUPTED                                    MAKE_NSHRESULTERROR (0x8006)  // Internal error found in the list. List is corrupted.
#define NS_E_AVCONTENT_IN_USE                                  MAKE_NSHRESULTERROR (0x8007)  // Invalid synchronization usage. Reset and Flush can't be performed if write and read count are not zero.
#define NS_E_AVCONTENT_NOT_FLUSHED                             MAKE_NSHRESULTERROR (0x8008)  // Alias reference can't be re-assigned if the AVContent hasn't been flushed.
#define NS_E_REFERENCE_NOT_ASSIGNED                            MAKE_NSHRESULTERROR (0x8009)  // This operation can't be performed since no user or alias reference has been assigned yet.
#define NS_E_DEVICE_NOT_PRESENT                                MAKE_NSHRESULTERROR (0x800A)  // The module needed for this operation is not properly installed or is not present on the current engine profile.
#define NS_E_SETTINGS_MANAGER_NOT_PRESENT                      MAKE_NSHRESULTERROR (0x800B)  // The settings manager is not properly installed.
#define NS_E_CLOCK_NOT_PRESENT                                 MAKE_NSHRESULTERROR (0x800C)  // The clock driver is not properly installed.
#define NS_E_CANNOT_CREATE_ENGINE_CREATOR                      MAKE_NSHRESULTERROR (0x800D)  // Flex Engine creator is not properly installed.
#define NS_E_INTERNAL_DIALOG_ERROR                             MAKE_NSHRESULTERROR (0x800E)  // The Flex Creator helper had an internal error. The main application should quit.
#define NS_E_USER_CANCELLED_DIALOG                             MAKE_NSHRESULTERROR (0x800F)  // The user clicked cancel in the profile or resoultion selection dialog. The main application should quit.
#define NS_E_NO_HARDWARE_PROFILE_FOUND                         MAKE_NSHRESULTERROR (0x8010)  // No hardware profile available in the system.
#define NS_E_NO_RESOURCE_FOUND                                 MAKE_NSHRESULTERROR (0x8011)  // Forgot to put <#include MvfHelper.rc> in the application's resource viewer|.RC properties|Resource Includes|Compile-time Directives
#define NS_E_END_OF_ENUM                                       MAKE_NSHRESULTERROR (0x8012)  // No more items to enumerate.
#define NS_E_NOT_SUPPORTED_BY_SURFACE_CONTAINER                MAKE_NSHRESULTERROR (0x8013)  // This surface operation can't be performed since the object is a surface container. A GetObject must
#define NS_E_INVALID_DEVICE_COUNT                              MAKE_NSHRESULTERROR (0x8014)  // The requested device is not installed. 
#define NS_E_INVALID_RESOLUTIONS_COUNT                         MAKE_NSHRESULTERROR (0x8015)  // The number or resolutions specified is different than the one expected.
#define NS_E_INRES_GENRES_INCOMPATIBLE_COMBINATION             MAKE_NSHRESULTERROR (0x8022)  // The input resolution is not compatible with the genlock resolution.
#define NS_E_OUTRES_GENRES_INCOMPATIBLE_COMBINATION            MAKE_NSHRESULTERROR (0x8023)  // The output resolution is not compatible with the genlock resolution
#define NS_E_MFACTOR_INCOMPATIBLE_COMBINATION                  MAKE_NSHRESULTERROR (0x8024)  // FrameRate M factors not compatible between genlock and input/output resolution
#define NS_E_INVALID_IODEVICE_COMBINATION                      MAKE_NSHRESULTERROR (0x8025)  // The combination of only one ABOB (or ABobSDI) and only one HDBOB is supported.
#define NS_E_INVALID_DEVICE_INDEX                              MAKE_NSHRESULTERROR (0x8026)  // The requested IO Device index is not valid.
#define NS_E_UNSUPPORTED_IOCOMMAND                             MAKE_NSHRESULTERROR (0x8027)  // The requested IO command is not supported.
#define NS_E_NO_FX_FOR_OUTPUT_SURFACE_LOCATION                 MAKE_NSHRESULTERROR (0x8028)  // The effect or composition can't process in the desired output surface memory location.
#define NS_E_DEVICE_MATCHING_FAILED                            MAKE_NSHRESULTERROR (0x8029)  // Can't match input and output device to IO device(s).
#define NS_E_MAX_FLEXENGINE_REACHED                            MAKE_NSHRESULTERROR (0x802B)  // No more Flex Engines can be created.
#define NS_E_ENGINE_ALREADY_CREATED                            MAKE_NSHRESULTERROR (0x802C)  // A Flex Engine is already created on the desired hardware profile.
#define NS_E_CO_INITIALIZE_NOT_DONE                            MAKE_NSHRESULTERROR (0x802D)  // Application must call COInitialize before using COM objects.
#define NS_E_TOO_MANY_INPUT_SURFACES                           MAKE_NSHRESULTERROR (0x802E)  // Allowed number of effect input surfaces is exceeded.
#define NS_E_MODULE_BUSY                                       MAKE_NSHRESULTERROR (0x802F)  // The module is still processing the last requested operation.
#define NS_E_NO_INPUT_SURFACE                                  MAKE_NSHRESULTERROR (0x8030)  // This method requires at least one input surface.
#define NS_E_NO_INPUT_CONTAINER                                MAKE_NSHRESULTERROR (0x8031)  // Effect needs effect pattern.
#define NS_E_READ_COUNT_NOT_INCREMENTED                        MAKE_NSHRESULTERROR (0x8032)  // Each SignalReadCompletion should match a IncrementReadCount.
#define NS_E_WRITE_COUNT_NOT_INCREMENTED                       MAKE_NSHRESULTERROR (0x8033)  // Each SignalWriteCompletion should match a IncrementWriteCount.
#define NS_E_MARKWRITE_ERROR                                   MAKE_NSHRESULTERROR (0x8034)  // MarkWriteStartTime not done OR MarkWriteStopTime not done OR MarkWriteStartTime done after MarkWriteStopTime.
#define NS_E_INRES_OUTRES_INCOMPATIBLE_COMBINATION             MAKE_NSHRESULTERROR (0x8035)  // The combination of input resolution and output resolution is invalid.
#define NS_E_NOT_READY_FOR_READ                                MAKE_NSHRESULTERROR (0x8036)  // The host processor module expects the sources to be NS_FALSE for read.
#define NS_E_COPY_FORMATS_COMBINATION_NOT_SUPPORTED            MAKE_NSHRESULTERROR (0x8037)  // The surface format of the source and destination surface imply a special copy that is not supported.
#define NS_E_COPY_RECTANGLES_MISMATCH                          MAKE_NSHRESULTERROR (0x8038)  // The source and destination rectangles specified are invalid or imply an unsupported copy.
#define NS_E_POINTER_NOT_ALIGNED                               MAKE_NSHRESULTERROR (0x8039)  // The data pointer of the source and/or destination surface is not properly aligned.
#define NS_E_FORMAT_NOT_SUPPORTED                              MAKE_NSHRESULTERROR (0x803A)  // The format of the surface or audiosamples are not supported in the requested processing.
#define NS_E_INPUT_FORMAT_NOT_SUPPORTED                        MAKE_NSHRESULTERROR (0x803B)  // The format of the input surface or audiosamples is not supported in the requested processing.
#define NS_E_OUTPUT_FORMAT_NOT_SUPPORTED                       MAKE_NSHRESULTERROR (0x803C)  // The format of the output surface or audiosamples is not supported in the requested processing.
#define NS_E_PROCESSING_FORMAT_NOT_SUPPORTED                   MAKE_NSHRESULTERROR (0x803D)  // The source and destination format imply a processing that is not supported.
#define NS_E_DESTINATION_BUFFER_TOO_SMALL                      MAKE_NSHRESULTERROR (0x803E)  // Destination buffer is too small.
#define NS_E_BUFFER_SIZE_MISMATCH                              MAKE_NSHRESULTERROR (0x803F)  // Input buffers (audiosamples) must have the same number of samples.
#define NS_E_BUFFER_FORMAT_MISMATCH                            MAKE_NSHRESULTERROR (0x8040)  // Input buffers or output buffers (audiosamples) must have the same format.
#define NS_E_IOCTL_FAILED                                      MAKE_NSHRESULTERROR (0x8041)  // A kernel driver IO control has failed.
#define NS_E_MUTEX_CREATION_FAILED                             MAKE_NSHRESULTERROR (0x8042)  // A mutex creation has failed.
#define NS_E_OUT_OF_MEMORY                                     MAKE_NSHRESULTERROR (0x8043)  // Not enough memory to perform the operation
#define NS_E_INVALID_PARAMETER                                 MAKE_NSHRESULTERROR (0x8044)  // One of the method parameter is invalid.
#define NS_E_INVALID_POINTER                                   MAKE_NSHRESULTERROR (0x8045)  // One of the method parameter is a uninitialized pointer.
#define NS_E_ALREADY_IN_USE                                    MAKE_NSHRESULTERROR (0x8046)  // Failed the module/object is already in use.
#define NS_E_NOT_INITIALIZED                                   MAKE_NSHRESULTERROR (0x8047)  // Failed because the module/object is not initialized.
#define NS_E_TIME_OUT                                          MAKE_NSHRESULTERROR (0x8048)  // Failed because there was a time out.
#define NS_E_WRONG_STATE                                       MAKE_NSHRESULTERROR (0x8049)  // Generic wrong state error, module/filter/object does not have the right state.
#define NS_E_INVALID_EFFECT_TREE                               MAKE_NSHRESULTERROR (0x804A)  // The specified effect tree cannot be used by the transform filter.
#define NS_E_INVALID_FILE_FORMAT                               MAKE_NSHRESULTERROR (0x804B)  // Unreadable file format.
#define NS_E_NO_PROFILE_LOADED                                 MAKE_NSHRESULTERROR (0x804C)  // Failed because no profile was selected on the filter graph.
#define NS_E_OUT_OF_RANGE                                      MAKE_NSHRESULTERROR (0x804D)  // Failed because a given value is out of range.
#define NS_E_HW_OVERLAY_NOT_AVAILABLE                          MAKE_NSHRESULTERROR (0x804E)  // Hardware overlay for live window display is not available.
#define NS_E_OUT_OF_DIRECTX_MEMORY                             MAKE_NSHRESULTERROR (0x804F)  // Failure when allocating DirectX memory.
#define NS_E_DIRECTX_IS_LOST                                   MAKE_NSHRESULTERROR (0x8050)  // Failure when the DirectX memory and context are lost.
#define NS_E_INVALID_NODE_RELATIONSHIP                         MAKE_NSHRESULTERROR (0x8051)  // Failure when trying to link tree nodes.
#define NS_E_ENUM_OUT_OF_SYNC			                           MAKE_NSHRESULTERROR (0x8052)  // Using a tree node enumerator out of synch with the current state of the tree.
#define NS_E_INVALID_NODE_INDEX		                           MAKE_NSHRESULTERROR (0x8053)  // Trying to force a node index on a reserved value.
#define NS_E_OUT_OF_POOL_MEMORY		                           MAKE_NSHRESULTERROR (0x8054)  // No more memory available in resource manager internal pool.
#define NS_E_END_OF_DISK       		                           MAKE_NSHRESULTERROR (0x8055)  // End Of Disk. 
#define NS_E_INVALID_DATA       		                           MAKE_NSHRESULTERROR (0x8056)  // Invalid data.  
#define NS_E_INVALID_INDEX       		                        MAKE_NSHRESULTERROR (0x8057)  // Invalid index.  
#define NS_E_WRONG_MEDIATYPE       		                        MAKE_NSHRESULTERROR (0x8058)  // Wrong MediaType. Could occur on SetCutlistManager() or pin connection.  
#define NS_E_WRONG_GRAPHMODE       		                        MAKE_NSHRESULTERROR (0x8059)  // Wrong Graph Mode. Could occur when sending Graph state commands.
#define NS_E_GRAPH_HAS_BEEN_BUILT                              MAKE_NSHRESULTERROR (0x805A)  // Graph has already been built. Could occur when user tries to set a number of source(sink, live window) filters after the graph has been built.
#define NS_E_GRAPH_DESTROYED                                   MAKE_NSHRESULTERROR (0x805b)  // GSC uses this to notify the souce loader threads to stop working.
#define NS_E_INVALID_NAME                                      MAKE_NSHRESULTERROR (0x805c)  // Invalid file name. 
#define NS_E_FILE_VERSION_NOT_SUPPORTED                        MAKE_NSHRESULTERROR (0x805D)  // If the file version is not supported.
#define NS_E_ELEMENT_IS_NOT_COMPATIBLE                         MAKE_NSHRESULTERROR (0x805E)  // The element is not compatible with the cutlist.
#define NS_E_ELEMENT_NOT_FOUND                                 MAKE_NSHRESULTERROR (0x805F)  // The specified element is not found in the cutlist.
#define NS_E_CUTLIST_ID_ALREADY_EXISTS                         MAKE_NSHRESULTERROR (0x8060)  // There has been a cutlist with the specified cutlist ID in the cutlist manager.
#define NS_E_CUTLIST_ID_NOT_FOUND		                        MAKE_NSHRESULTERROR (0x8061)  // There is no cutlist with the specified cutlist ID in the cutlist manager.
#define NS_E_WRONG_ERES                                        MAKE_NSHRESULTERROR (0x8062)  // The editing resolution doesn't match the one set in the profile loader.
#define NS_E_POS_NOT_AT_BOUNDARY                               MAKE_NSHRESULTERROR (0x8063)  // The timeline position is not at the boundary of an element.
#define NS_E_POS_NOT_BETWEEN_TWO_SPLIT_ELEMENTS                MAKE_NSHRESULTERROR (0x8064)  // The timeline position is not at the boundary of two split elements.
#define NS_E_EMPTY_RANGE                                       MAKE_NSHRESULTERROR (0x8065)  // The start point and the end point of the specified range are equal.
#define NS_E_CANNOT_CANCEL                                     MAKE_NSHRESULTERROR (0x8066)  // Cannot cancel a surface when it's delivered too late to respect the preroll time of output device.
#define NS_E_INVALID_TASK                                   MAKE_NSHRESULTERROR (0x8067)  // A processing task is invalid.
#define NS_E_TASK_NOT_READY                                 MAKE_NSHRESULTERROR (0x8068)  // A processing task is not ready to execute.
#define NS_E_TASK_NOT_COMPLETED                             MAKE_NSHRESULTERROR (0x8069)  // A processing task is not completed.
#define NS_E_TASK_NOT_FLUSHED                               MAKE_NSHRESULTERROR (0x806A)  // A processing task is not flushed.
#define NS_E_ENGINE_ALREADY_REGISTERED                         MAKE_NSHRESULTERROR (0x806B)  // A streaming manager is already registered on the task execution manager.
#define NS_E_TASK_REJECTED                                  MAKE_NSHRESULTERROR (0x806C)  // A processing request was refused because the scheduler is flushing itself.
#define NS_E_POOL_FULL                                         MAKE_NSHRESULTERROR (0x806D)  // Adding a new element to a pool is refused because the pool is full.
#define NS_E_UNKNOWN_EXCEPTION                                 MAKE_NSHRESULTERROR (0x806E)  // An unknown exception was caught by the call.
#define NS_E_MAX_VALUE_REACHED                                 MAKE_NSHRESULTERROR (0x806F)  // There's a maximum value for that and it has been reached.  
#define NS_E_INCONSISTENCY_DETECTED                            MAKE_NSHRESULTERROR (0x8070)  // There's an inconsistency that was detected by the object.
#define NS_E_ARRAY_TOO_SMALL                                   MAKE_NSHRESULTERROR (0x8071)  // Provided array is too small.
#define NS_E_NODE_INPUT_NOTREADY                               MAKE_NSHRESULTERROR (0x8072)  // Input to a node is not ready.
#define NS_E_INVALID_HANDLE                                    MAKE_NSHRESULTERROR (0x8073)  // Handle is invalid.
#define NS_E_DIRECTX_LOCK                                      MAKE_NSHRESULTERROR (0x8074)  // Failed to lock some DirectX memory.
#define NS_E_BUFFER_REQUEST_TOO_LATE                           MAKE_NSHRESULTERROR (0x8075)  // The requested buffer could not be processed because it's too late.
#define NS_E_BUFFER_REQUEST_PARTIAL_TOO_LATE                   MAKE_NSHRESULTERROR (0x8076)  // The requested buffer could only be processed partially. The rest was too late.
#define NS_E_SURFACE_FORMAT_MISMATCH                           MAKE_NSHRESULTERROR (0x8077)  // The operation's in/out surface format combination is not supported.
#define NS_E_READ_ACCESS_ONLY                                  MAKE_NSHRESULTERROR (0x8078)  // The alias access type is read-only and therefore, can't be used in write access.
#define NS_E_NOT_AVAILABLE                                     MAKE_NSHRESULTERROR (0x8079)  // The requested data are not available.
#define NS_E_SYNCHRO_ACCESS_NOT_ENABLED                        MAKE_NSHRESULTERROR (0x807A)  // The AVContent synchronization access is disabled. Occur when a module tries to access an AVContent that has been recycled and returned to the free pool.
#define NS_E_HD_BOB_NOT_PRESENT                                MAKE_NSHRESULTERROR (0x807B)  // HD Resolutions are not supported since there's no HD BOB present in the system.
#define NS_E_AUDIO_MIXER_NOT_PRESENT                           MAKE_NSHRESULTERROR (0x807C)  // An error occured during the initialization of the AudioMixer engine. 
#define NS_E_BUFFER_ENGINE_NOT_PRESENT                         MAKE_NSHRESULTERROR (0x807D)  // An error occured during the initialization of the host buffer engine. 
#define NS_E_SLOW_MOTION_NOT_PRESENT                           MAKE_NSHRESULTERROR (0x807E)  // An error occured during the initialization of the slow motion module. 
#define NS_E_AUDIO_SAMPLES_ENGINE_NOT_PRESENT                  MAKE_NSHRESULTERROR (0x807F)  // An error occured during the initialization of the audio samples engine. 
#define NS_E_FLEX3D_DEVICE_NOT_PRESENT                         MAKE_NSHRESULTERROR (0x8080)  // An error occured during the initialization of the Flex3D Device. 
#define NS_E_COMPOSITOR_NOT_PRESENT                            MAKE_NSHRESULTERROR (0x8081)  // An error occured during the initialization of the compositor. 
#define NS_E_INPUT_OUTPUT_NOT_PRESENT                          MAKE_NSHRESULTERROR (0x8082)  // An error occured during the initialization of the input/output module. 
#define NS_E_HARDWARE_INFO_NOT_PRESENT                         MAKE_NSHRESULTERROR (0x8083)  // An error occured during the initialization of the hardware info module. 
#define NS_E_KERNEL_DRIVER_NOT_PRESENT                         MAKE_NSHRESULTERROR (0x8084)  // One of the required kernel driver (.sys) is not properly installed. 
#define NS_E_INVALID_PROFILE                                   MAKE_NSHRESULTERROR (0x8085)  // An invalid profile type was selected.
#define NS_E_INVALID_MONITOR                                   MAKE_NSHRESULTERROR (0x8086)  // Invalid monitor combination detected (live window).
#define NS_E_PCI_OPTIMIZER_NOT_PRESENT                         MAKE_NSHRESULTERROR (0x8087)  // An error occured during the initialization of the PCI optimizer module.
#define NS_E_NOT_ENOUGH_INPUT_SURFACES                         MAKE_NSHRESULTERROR (0x8088)  // Not enough surface were send to the effect
#define NS_E_NOT_ENOUGH_INPUT_CONTAINER                        MAKE_NSHRESULTERROR (0x8089)  // Not enough container were send to the effect
#define NS_E_TOO_MANY_INPUT_CONTAINER                          MAKE_NSHRESULTERROR (0x808A)  // Too many container were send to the effect
#define NS_E_HW_RENDERER_CREATION_FAILED                       MAKE_NSHRESULTERROR (0x808B)  // The creation of the hardware implementation of the effect failed.
#define NS_E_OUTSIDE_VISIBLE_AREA                              MAKE_NSHRESULTERROR (0x808C)  // Failed because a window is outside the visible area.
#define NS_E_FILE_NOT_FOUND                                    MAKE_NSHRESULTERROR (0x808D)  // Failed because a file was not found.
#define NS_E_NO_COMPLETION_EVENTS                              MAKE_NSHRESULTERROR (0x808E)  // Error in main DirectShow scheduler because no completion events are available for the scheduler.
#define NS_E_NO_EXECUTION_EVENTS                               MAKE_NSHRESULTERROR (0x808F)  // Error in main DirectShow scheduler because no execution events are available for the scheduler.
#define NS_E_NO_MEMORY_AVAILABILITY_EVENTS                     MAKE_NSHRESULTERROR (0x8090)  // Error in main DirectShow scheduler because no memory availability events are available for the scheduler.

#define NS_E_UNSUPPORTED_GENLOCK_WIDTH_HEIGHT                  MAKE_NSHRESULTERROR (0x8091)  // The requested genlock width and/or height is not supported.
#define NS_E_UNSUPPORTED_INPUT_WIDTH_HEIGHT                    MAKE_NSHRESULTERROR (0x8092)  // The requested input width and/or height is not supported.
#define NS_E_UNSUPPORTED_INPUT1_WIDTH_HEIGHT                   MAKE_NSHRESULTERROR (0x8093)  // The requested input width and/or height is not supported.
#define NS_E_UNSUPPORTED_INPUT2_WIDTH_HEIGHT                   MAKE_NSHRESULTERROR (0x8094)  // The requested input width and/or height is not supported.
#define NS_E_UNSUPPORTED_OUTPUT_WIDTH_HEIGHT                   MAKE_NSHRESULTERROR (0x8095)  // The requested output width and/or height is not supported.
#define NS_E_UNSUPPORTED_OUTPUT1_WIDTH_HEIGHT                  MAKE_NSHRESULTERROR (0x8096)  // The requested output width and/or height is not supported.
#define NS_E_UNSUPPORTED_OUTPUT2_WIDTH_HEIGHT                  MAKE_NSHRESULTERROR (0x8097)  // The requested output width and/or height is not supported.
#define NS_E_UNSUPPORTED_GENLOCK_FRAMERATE_SCANMODE            MAKE_NSHRESULTERROR (0x8098)  // The requested genlock frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_INPUT_FRAMERATE_SCANMODE              MAKE_NSHRESULTERROR (0x8099)  // The requested input frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_INPUT1_FRAMERATE_SCANMODE             MAKE_NSHRESULTERROR (0x809A)  // The requested input frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_INPUT2_FRAMERATE_SCANMODE             MAKE_NSHRESULTERROR (0x809B)  // The requested input frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_OUTPUT_FRAMERATE_SCANMODE             MAKE_NSHRESULTERROR (0x809C)  // The requested output frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_OUTPUT1_FRAMERATE_SCANMODE            MAKE_NSHRESULTERROR (0x809D)  // The requested output frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_OUTPUT2_FRAMERATE_SCANMODE            MAKE_NSHRESULTERROR (0x809E)  // The requested output frame rate and/or scan mode is not supported.
#define NS_E_UNSUPPORTED_GENLOCK_ASPECT_RATIO                  MAKE_NSHRESULTERROR (0x809F)  // The requested genlock aspect ratio is not supported.
#define NS_E_UNSUPPORTED_INPUT_ASPECT_RATIO                    MAKE_NSHRESULTERROR (0x80A0)  // The requested input aspect ratio is not supported.
#define NS_E_UNSUPPORTED_INPUT1_ASPECT_RATIO                   MAKE_NSHRESULTERROR (0x80A1)  // The requested input aspect ratio is not supported.
#define NS_E_UNSUPPORTED_INPUT2_ASPECT_RATIO                   MAKE_NSHRESULTERROR (0x80A2)  // The requested input aspect ratio is not supported.
#define NS_E_UNSUPPORTED_OUTPUT_ASPECT_RATIO                   MAKE_NSHRESULTERROR (0x80A3)  // The requested output aspect ratio is not supported.
#define NS_E_UNSUPPORTED_OUTPUT1_ASPECT_RATIO                  MAKE_NSHRESULTERROR (0x80A4)  // The requested output aspect ratio is not supported.
#define NS_E_UNSUPPORTED_OUTPUT2_ASPECT_RATIO                  MAKE_NSHRESULTERROR (0x80A5)  // The requested output aspect ratio is not supported.

#define NS_E_UIF_VIDEO_CAPTURE_START_FAILED                    MAKE_NSHRESULTERROR (0x80A6)  // The UIF video capture returned an error for the start.
#define NS_E_UIF_AUDIO_CAPTURE_START_FAILED                    MAKE_NSHRESULTERROR (0x80A7)  // The UIF audio capture returned an error for the start.
#define NS_E_UIF_VIDEO_TIMESTAMP_ZERO                          MAKE_NSHRESULTERROR (0x80A8)  // The UIF video capture buffer returned a buffer with timestamp 0.
#define NS_E_UIF_AUDIO_TIMESTAMP_ZERO                          MAKE_NSHRESULTERROR (0x80A9)  // The UIF audio capture returned a buffer with timestamp 0.
#define NS_E_UIF_OUT_OF_HOSTBUFFERS                            MAKE_NSHRESULTERROR (0x80AA)  // The UIF ran out of host buffers to copy the onboard buffers.

#define NS_E_INVALID_PROCESSING_JOB_STATE                      MAKE_NSHRESULTERROR (0x80AB)  // Scheduler has detected an invalid processing job state.
#define NS_E_PROCESSING_JOB_NOT_READY                          MAKE_NSHRESULTERROR (0x80AC)  // Scheduler has a problem with a job not ready.
#define NS_E_PROCESSING_JOB_NOT_PREPARED_FOR_EXECUTION         MAKE_NSHRESULTERROR (0x80AD)  // Scheduler has a problem with a job not prepared for execution.
#define NS_E_PROCESSING_JOB_NOT_EXECUTING                      MAKE_NSHRESULTERROR (0x80AE)  // Scheduler has a problem with a job not executing.
#define NS_E_PROCESSING_JOB_NOT_COMPLETED                      MAKE_NSHRESULTERROR (0x80AF)  // Scheduler has a problem with a job not completed.
#define NS_E_PROCESSING_JOB_EXECUTING                          MAKE_NSHRESULTERROR (0x80B0)  // Scheduler has a problem with a job executing.
#define NS_E_PROCESSING_JOB_COMPLETED                          MAKE_NSHRESULTERROR (0x80B1)  // Scheduler has a problem with a job completed.
#define NS_E_PROCESSING_JOB_REMAINING                          MAKE_NSHRESULTERROR (0x80B2)  // Scheduler has a problem with some remaining jobs.
#define NS_E_GRAPH_HAS_NOT_BEEN_DISASSEMBLED                   MAKE_NSHRESULTERROR (0x80B3)  // Graph has not been disassembled.
#define NS_E_FILTER_NOT_IN_GRAPH                               MAKE_NSHRESULTERROR (0x80B4)  // Graph has probably already been disassembled.
#define NS_E_PROCESSING_NOT_SUPPORTED_IN_CONFIGURATION         MAKE_NSHRESULTERROR (0x80B5)  // Processing in not supported in the current configuration
#define NS_E_INVALID_BITRATE                                   MAKE_NSHRESULTERROR (0x80B6)  // Invalid bit rate in codec for compression
#define NS_E_INVALID_CUTLIST                                   MAKE_NSHRESULTERROR (0x80B7)  // Cutlist parameters are invalid for the given file
#define NS_E_GFX_FILE_IS_REQUIRED                              MAKE_NSHRESULTERROR (0x80B8)  // There's no GFX file provided
#define NS_E_DV_AUDIO_ABSTRACTOR_NOT_PRESENT                   MAKE_NSHRESULTERROR (0x80B9)  // An error occured during the initialization of the DV Audio Abstractor module. 
#define NS_E_MEMORY_REGIONS_ALLOCTED_TOO_MANY                  MAKE_NSHRESULTERROR (0x80Ba)  // Too many memory regions allocated in mvdcache.dll , it's abnormal .

#define NS_E_INVALID_GOP                                       MAKE_NSHRESULTERROR (0x80BB)  // General error reported by the codec for a GOP
#define NS_E_OPEN_GOP_NOT_SUPPORTED                            MAKE_NSHRESULTERROR (0x80BC)  // This type of GOP is not yet supported.
#define NS_E_GOP_TOO_BIG                                       MAKE_NSHRESULTERROR (0x80BD)  // More than 30 frames in a GOP. This is a limit inside the codec. It can be change to something bigger.

#define NS_E_CODEC_INFO_NOT_AVAILABLE                          MAKE_NSHRESULTERROR (0x80BE)  // The codec info is not avilable. No validation of file format can be done.
#define NS_E_VIDEO_DECODER_NOT_AVAILABLE                       MAKE_NSHRESULTERROR (0x80BF)  // The required video decoder is not available. This error code is returned for video only file. This file can not be played.
#define NS_E_AUDIO_DECODER_NOT_AVAILABLE                       MAKE_NSHRESULTERROR (0x80C0)  // The required audio decoder is not available. This error code is returned for audio only file. This file can not be played.
#define NS_E_AVI_NOT_SUPPORTED                                 MAKE_NSHRESULTERROR (0x80C1)  // The interleaved AV file info is known to matrox file reader but the Video and audio formats are not supported. This warning code is only returned for interleaved AV file.  This file can not be played.
#define NS_E_FILE_FORMAT_NOT_SUPPORTED                         MAKE_NSHRESULTERROR (0x80C2)  // The file info is unknown to matrox file reader (eg: a txt file); the file information could not be retried on this file. The file info structure is invalid and should not be used.  This file can not be played.
#define NS_E_ERES_NOT_SET                                      MAKE_NSHRESULTERROR (0x80C3)  // The requested check can not be completed because there is no valid resolution setting currently set.
#define NS_E_INCOMPATIBLE_WIDTH                                MAKE_NSHRESULTERROR (0x80C4)  // The width of the file and the editing resolution is incompatible. The file can not be streamed.
#define NS_E_INCOMPATIBLE_HEIGHT                               MAKE_NSHRESULTERROR (0x80C5)  // The height of the file and the editing resolution is incompatible. The file can not be streamed.
#define NS_E_INCOMPATIBLE_COMPONENT_BIT_COUNT                  MAKE_NSHRESULTERROR (0x80C6)  // The component bit count of the file and the editing resolution is incompatible. The file can not be streamed.
#define NS_E_INCOMPATIBLE_FRAME_RATE                           MAKE_NSHRESULTERROR (0x80C7)  // The frame rate of the file and the editing resolution is incompatible. The file can not be streamed.

#define NS_E_UNSUPPORTED_DEPTH                                 MAKE_NSHRESULTERROR (0x80C8)  // The value for the depth of the surface description is not supported
#define NS_E_UNSUPPORTED_SURFACE_TYPE                          MAKE_NSHRESULTERROR (0x80C9)  // The value for the surface type of the surface description is not supported
#define NS_E_UNSUPPORTED_MEMORY_LOCATION                       MAKE_NSHRESULTERROR (0x80CA)  // The value for the memory location of the surface description is not supported
#define NS_E_INVALID_POLARITY                                  MAKE_NSHRESULTERROR (0x80CB)  // The value for the polarity of the surface description is not valid for the standard used
#define NS_E_UNSUPPORTED_IMAGE_ORIGIN                          MAKE_NSHRESULTERROR (0x80CC)  // The value for the image origin of the surface description is not supported
#define NS_E_SHAPED_VIDEO_NOT_SUPPORTED                        MAKE_NSHRESULTERROR (0x80CD)  // Shaped video is not supported in this module
#define NS_E_UNSUPPORTED_SURFACE_FORMAT                        MAKE_NSHRESULTERROR (0x80CE)  // The value for the surface format of the surface description is not supported
#define NS_E_UNSUPPORTED_COMPONENT_BIT_COUNT                   MAKE_NSHRESULTERROR (0x80CF)  // The value for the component bit count of the surface description is not supported
#define NS_E_SURFACE_WIDTH_MISMATCH                            MAKE_NSHRESULTERROR (0x80D0)  // The width of the input and output surface are not the same
#define NS_E_INVALID_PITCH                                     MAKE_NSHRESULTERROR (0x80D1)  // The pitch does not match the minimum requirement
#define NS_E_SOURCE_BUFFER_TOO_SMALL                           MAKE_NSHRESULTERROR (0x80D2)  // The source buffer is smaller than what it should be
#define NS_E_POLARITY_TOP_LINE_MISMATCH                        MAKE_NSHRESULTERROR (0x80D3)  // The top line value of the surface description does not match the polarity
#define NS_E_INVALID_COMPRESSED_DATA                           MAKE_NSHRESULTERROR (0x80D4)  // The compressed data received if not valid
#define NS_E_SURFACE_HEIGHT_MISMATCH                           MAKE_NSHRESULTERROR (0x80D5)  // The height of the input and output surface are not the same

#define NS_E_WRONG_EDIT_MODE                                   MAKE_NSHRESULTERROR (0x80D6)  // The device control is in the wrong edit mode.
#define NS_E_EDIT_SET_ACTIVE                                   MAKE_NSHRESULTERROR (0x80D7)  // The edit was already set active.
#define NS_E_DEVICE_NOT_CUED                                   MAKE_NSHRESULTERROR (0x80D8)  // The device was not cued.

#define NS_E_LOST_VIDEO_INPUT                                  MAKE_NSHRESULTERROR (0x80D9)  // The video input is lost
#define NS_E_LOST_EMBEDDED_AUDIO_INPUT                         MAKE_NSHRESULTERROR (0x80DA)  // The embedded audio input is lost
#define NS_E_LOST_GENLOCK                                      MAKE_NSHRESULTERROR (0x80DB)  // The genlock was lost.
#define NS_E_GENLOCK_CHANGED                                   MAKE_NSHRESULTERROR (0x80DC)  // The genlock has changed.

#define NS_E_CORRUPTION_DETECTED                               MAKE_NSHRESULTERROR (0x80DD)  // The buffer that was decompressed was corrupted.  The video data outputted may be incomplete.

#define NS_E_XLINK_DISCONNECTED					     	      MAKE_NSHRESULTERROR (0x80DE)  // The xlink has been disconnected.	

#define NS_E_GOP_POSITION_INVALID                              MAKE_NSHRESULTERROR (0x80DF)  // The IBP codec was asked to decode at a position greater than the number of frame in the GOP.
#define NS_E_INPOINT_NOTSET                                    MAKE_NSHRESULTERROR (0x80E0)  // An inpoint timecode was not set for the device control.
#define NS_E_OUTPOINT_NOTSET                                   MAKE_NSHRESULTERROR (0x80E1)  // An outpoint timecode was not set for the device control.

#define NS_E_VIDEO_INPUT_ABSENT                                MAKE_NSHRESULTERROR (0x80E2)  // The video input is not present.
#define NS_E_INPUT_RESOLUTION_MISMATCH                         MAKE_NSHRESULTERROR (0x80E3)  // The input resolution mismatches.

#define NS_E_FILE_ALREADY_OPENED                               MAKE_NSHRESULTERROR (0x80E4)  // This file has already been subject to a OpenFile
#define NS_E_INVALID_CAPTURE_TYPE                              MAKE_NSHRESULTERROR (0x80E5)  // This capture type is not supported
#define NS_E_VIDEO_ENCODER_NOT_AVAILABLE                       MAKE_NSHRESULTERROR (0x80E6)  // We do not have a codec for the requested capture type
#define NS_E_UNSUPPORTED_ENCODER_RESOLUTION                    MAKE_NSHRESULTERROR (0x80E7)  // The specified resolution is not supported by the video codec
#define NS_E_FILENAME_NOT_SPECIFIED                            MAKE_NSHRESULTERROR (0x80E8)  // We were asked to open a file but no filename has been supplied
#define NS_E_INVALID_START_POSITION                            MAKE_NSHRESULTERROR (0x80E9)  // The requested start position inside the file is invalid
#define NS_E_INVALID_FILE_OPEN_MODE                            MAKE_NSHRESULTERROR (0x80EA)  // This file open mode is not supported
#define NS_E_TDIR_NOT_SUPPORTED                                MAKE_NSHRESULTERROR (0x80EB)  // This feature is not supported
#define NS_E_CUSTOMIO_NOT_SUPPORTED                            MAKE_NSHRESULTERROR (0x80EC)  // This feature is not supported
#define NS_E_STRUCTURE_SIZE_MISMATCH                           MAKE_NSHRESULTERROR (0x80ED)  // The input structure is of an unexpected size
#define NS_E_UNSUPPORTED_SCANMODE                              MAKE_NSHRESULTERROR (0x80EE)  // The scanmode is not supported
#define NS_E_UNSUPPORTED_FRAMERATE                             MAKE_NSHRESULTERROR (0x80EF)  // The framerate is not supported
#define NS_E_UNSUPPORTED_ASPECT_RATIO                          MAKE_NSHRESULTERROR (0x80F0)  // The aspect ratio is not supported
#define NS_E_INVALID_PIPELINE_SIZE                             MAKE_NSHRESULTERROR (0x80F1)  // The pipeline size is not supported
#define NS_E_FILE_NOT_OPENED                                   MAKE_NSHRESULTERROR (0x80F2)  // No OpenFile was performed
#define NS_E_NOT_ENOUGH_INPUT_SAMPLES                          MAKE_NSHRESULTERROR (0x80F3)  // Not enough input audio samples

#define NS_E_GRAPH_MUST_BE_STOPPED                             MAKE_NSHRESULTERROR (0x80F4)  // Tried to add or remove window when graph was not stopped

#define NS_E_INCOMPATIBLE_SCAN_MODE                            MAKE_NSHRESULTERROR (0x80F5)  // The scan mode of the file and the editing resolution is incompatible. The file can not be streamed.
#define NS_E_INVALID_AVIODML_INDEX                             MAKE_NSHRESULTERROR (0x80F6)  // The index of the AVI Open DML file is invalid
#define NS_E_PARAMETER_ARRAY_TOO_BIG                           MAKE_NSHRESULTERROR (0x80F7)  // Parameter array too big
#define NS_E_UNSUPPORTED_AUDIO_CHANNEL_TYPE                    MAKE_NSHRESULTERROR (0x80F8)  // The audio channel type (number of tracks) is not supported

#define NS_E_MUST_GET_COPY_SIZE                                MAKE_NSHRESULTERROR (0x80F9)  // GetSizeOfBufferToAllocateForHostCopy must be called to set the size
#define NS_E_CANNOT_COPY_WHEN_RUNNING                          MAKE_NSHRESULTERROR (0x80FA)  // CopyLastOutputRGBAToHost cannot be called while the graph is running
#define NS_E_INVALID_THREAD_PRIORITY                           MAKE_NSHRESULTERROR (0x80FB)  // The specified thread priority is not within the expected range
#define NS_E_WAV_FILE_SIZE_LIMIT_REACHED                       MAKE_NSHRESULTERROR (0x80FC)  // The .wav file size limit (4 GB) as been reached, cannot write more data 
#define NS_E_DEVICE_HAS_BEEN_OPENED                            MAKE_NSHRESULTERROR (0x80FD)  // The VCR device has been opened. 
#define NS_E_UNSUPPORTED_VBI_RESOLUTION                        MAKE_NSHRESULTERROR (0x80FE)  // The specified VBI resolution is not supported. 
#define NS_E_DROPPED_SAMPLE                                    MAKE_NSHRESULTERROR (0x80FF)  // A sample (field or frame, audio or video) has been dropped. 
#define NS_E_APPEND_SECTOR_SIZE_DIFFERENT                      MAKE_NSHRESULTERROR (0x8100)  // On appending , the file has been copied to a drive that has different sector size  to the original drive.
#define NS_E_APPEND_FORMAT_NOT_COMPATIBLE                      MAKE_NSHRESULTERROR (0x8101)  // On appending ,  foramt of new data is not compatible of old data , muxer is not set properly. 
#define NS_E_TRUNCATE_POSITION_INVALID                         MAKE_NSHRESULTERROR (0x8102)  // On truncation , the required truncation position is not in the file. 
#define NS_E_PLAYLIST_NOT_SET                                  MAKE_NSHRESULTERROR (0x8103)  // Playlist has not been set.
#define NS_E_PIPELINE_SIZE_NOT_SET                             MAKE_NSHRESULTERROR (0x8105)  // Pipeline size has not been set.
#define NS_E_INVALID_PLAYLIST_TYPE                             MAKE_NSHRESULTERROR (0x8106)  // Playlist type is invalid.
#define NS_E_POSITION_PARAMETER_NOT_SET                        MAKE_NSHRESULTERROR (0x8107)  // Position parameter is NULL.
#define NS_E_VIDEO_SURFACES_PARAMETER_NOT_SET                  MAKE_NSHRESULTERROR (0x8108)  // Video surfaces parameter is NULL.
#define NS_E_POOL_ALLOCATION_FAILED                            MAKE_NSHRESULTERROR (0x8109)  // Unable to allocate a specific pool.
#define NS_E_FAILED_TO_RETRIEVE_POSITION_FROM_DISK             MAKE_NSHRESULTERROR (0x810A)  // Specified position was not expected to be read from disk.
#define NS_E_UNSUPPORTED_COMPRESSION_TYPE                      MAKE_NSHRESULTERROR (0x810B)  // Specified compression format is not supported.
#define NS_E_CODEC_OPERATION_FAILED                            MAKE_NSHRESULTERROR (0x810C)  // A codec operation failed with an unknown error.
#define NS_E_AUDIO_SAMPLES_PARAMETER_NOT_SET                   MAKE_NSHRESULTERROR (0x810D)  // Audio samples parameter is NULL.
#define NS_E_TOO_MANY_INPUT_SAMPLES                            MAKE_NSHRESULTERROR (0x810E)  // Caller is expecting too many audio samples in array parameter.
#define NS_E_INPUT_SAMPLES_ARRAY_SIZE_MISMATCH                 MAKE_NSHRESULTERROR (0x810F)  // Caller is changing number of expected samples array size between calls.
#define NS_E_AUDIO_DUPLICATE_FAILED                            MAKE_NSHRESULTERROR (0x8110)  // Audio duplication failed with an unknown error.
#define NS_E_UNABLE_TO_RETRIEVE_AUDIO_SAMPLES                  MAKE_NSHRESULTERROR (0x8111)  // Unable to retrieve audio samples probably due to pipeline size being insufficient.
#define NS_E_TIMEOUT_WAITING_FOR_DISK                          MAKE_NSHRESULTERROR (0x8112)  // Timeout while waiting for a file IO of a specific position.
#define NS_E_CANNOT_CHANGE_DEVICE_WHEN_RUNNING                 MAKE_NSHRESULTERROR (0x8113)  // The graph must be stopped to change the input device. 

#define NS_E_ELEMENT_IN_USE                                    MAKE_NSHRESULTERROR (0x8114)  // Play list element is already in use 
#define NS_E_ELEMENT_NOT_LOCKED                                MAKE_NSHRESULTERROR (0x8115)  // Play list element was not locked
#define NS_E_ELEMENT_LOST                                      MAKE_NSHRESULTERROR (0x8116)  // Play list element was not moved in the play list and is NOT in it anymore
#define NS_E_INVALID_ELEMENT_TYPE                              MAKE_NSHRESULTERROR (0x8117)  // Play list element is not the same type as the play list
#define NS_E_INVALID_PATH_SIZE                                 MAKE_NSHRESULTERROR (0x8118)  // Filename size is too big.
#define NS_E_ELEMENT_ALREADY_IN_PLAYLIST                       MAKE_NSHRESULTERROR (0x8119)  // Play list element is already in the list.
#define NS_E_ELEMENT_FOUND                                     MAKE_NSHRESULTERROR (0x8120)  // A play list element was found at the specified location.
#define NS_E_UNSUPPORTED_AUDIO_DATA_TYPE                       MAKE_NSHRESULTERROR (0x8121)  // The audio data type is not supported on the selected profile
#define NS_E_HARDDISK_ACCESS_FAILURE                           MAKE_NSHRESULTERROR (0x8122)  // Harddisk access failed.
#define NS_E_HARDDISK_TOO_SLOW                                 MAKE_NSHRESULTERROR (0x8123)  // Harddisk is too slow.

#define NS_E_PIPES_ALREADY_LINKED                              MAKE_NSHRESULTERROR (0x8124)  // The pipes are already connected or already have a trig link.
#define NS_E_PIPES_NOT_CONNECTED                               MAKE_NSHRESULTERROR (0x8125)  // The pipes are not connected to one another.
#define NS_E_TOO_MANY_UPSTREAM_PIPES                           MAKE_NSHRESULTERROR (0x8126)  // The maximum number of upstream pipes has been reached.
#define NS_E_TPIA_SMALLER_THAN_TPOA                            MAKE_NSHRESULTERROR (0x8127)  // The target advance at a pipe's input cannot be smaller than the target advance at its output.
#define NS_E_MAXPIPEDEPTH_SMALLER_THAN_ONE                     MAKE_NSHRESULTERROR (0x8128)  // The maximum pipe depth must be greater than 0.
#define NS_E_ENDOFPIPE_WAS_RECEIVED                            MAKE_NSHRESULTERROR (0x8129)  // The EndOfPipe pipe element was received, preventing the requested operation from being executed.
#define NS_E_ENDOFPIPE_WAS_PROCESSED                           MAKE_NSHRESULTERROR (0x812A)  // The EndOfPipe pipe element was processed, preventing the requested operation from being executed.
#define NS_E_INCOMPATIBLE_PIPE_ELEMENT                         MAKE_NSHRESULTERROR (0x812B)  // The pipe element is incompatible with the pipe.
#define NS_E_CANCELLATION_TOO_EARLY                            MAKE_NSHRESULTERROR (0x812C)  // The pipe has not yet reached the position specified for the cancellation (TrigDur or ShuttlingSpeed changed too soon).
#define NS_E_PROBLEM_TALKING_TO_UPSTREAM_PIPE                  MAKE_NSHRESULTERROR (0x812D)  // The pipe had problem talking to an upstream pipe. See the upstream's pipe error.
#define NS_E_PROBLEM_TALKING_TO_DOWNSTREAM_PIPE                MAKE_NSHRESULTERROR (0x812E)  // The pipe had problem talking to a downstream pipe. See the downstream's pipe error.
#define NS_E_INVALID_INSTANTFRAMERATE                          MAKE_NSHRESULTERROR (0x812F)  // Some fields of an InstantFrameRate object are invalid.
#define NS_E_PROBLEM_IN_SUB_PROCESSING                         MAKE_NSHRESULTERROR (0x8130)  // The pipe had problem in the sub-processing of a Trig, a ChangeTrigDur, or an EndOfPipe.
#define NS_E_BAD_PIPE_CONFIGURATION                            MAKE_NSHRESULTERROR (0x8131)  // The pipe is badly configured.
#define NS_E_BAD_PIPE_STATE                                    MAKE_NSHRESULTERROR (0x8132)  // The pipe is in a bad state.
#define NS_E_FAILED_TO_PROCESS_PIPE_ELEMENT                    MAKE_NSHRESULTERROR (0x8133)  // A pipe was not able to properly process a pipe element.
#define NS_E_ALREADY_PROCESSING_PIPE_ELEMENT                   MAKE_NSHRESULTERROR (0x8134)  // A pipe was already processing a pipe element when it was asked to work on another one.
#define NS_E_PROCESSBEST_PIPE_DOESNT_PROPAGATE_TRIGS           MAKE_NSHRESULTERROR (0x8135)  // The ProcessBest processing pipe doesn't support to forward trigs to other pipes.

#define NS_E_TRIM_LENGTH_LARGER_THAN_MEDIA_LENGTH              MAKE_NSHRESULTERROR (0x8136)  // The playlist element's trim length exceeds the media's length
#define NS_E_TRIM_OUTSIDE_MEDIA_RANGE                          MAKE_NSHRESULTERROR (0x8137)  // The playlist element's trim infos are out of the media's boundaries
#define NS_E_TRIM_LENGTH_NOT_SUPPORTED                         MAKE_NSHRESULTERROR (0x8138)  // The playlist element's trim length is invalid (ex: zero, or audio and not 1.0)
#define NS_E_DURATION_LENGTH_NOT_SUPPORTED                     MAKE_NSHRESULTERROR (0x8139)  // The playlist element's duration length is invalid (ex: zero)
#define NS_E_TASK_FLUSHED                                      MAKE_NSHRESULTERROR (0x813A)  // Operation on request failed because request is already completed and flushed.
#define NS_E_FILEHANDLE_ALREADY_SET                            MAKE_NSHRESULTERROR (0x813B)  // SetFileHandle on a file handle object failed because the file handle has been set.
#define NS_E_COUNT_OF_LIST_BUSY_BLOCKS_NON_0                   MAKE_NSHRESULTERROR (0x813C)  // In mvdcache , count of list busy blocks is not 0 . This is wrong when FlushCachedInformation is called on blockallocator.
#define NS_E_OUT_OF_ORDER                                      MAKE_NSHRESULTERROR (0x813D)  // This is when a strict order is not respected.
#define NS_E_FILEACCESS_OPENFILE_FAILED                        MAKE_NSHRESULTERROR (0x813E)  // File open failed in CMvdFileAccess module.
#define NS_E_FILEACCESS_GET_FILE_SIZE_FAILED                   MAKE_NSHRESULTERROR (0x813F)  // GetFileSize failed in CMvdFileAccess module.
#define NS_E_REG_SETVALUE_FAIL			     			       MAKE_NSHRESULTERROR (0x8140)  // Set regitry value fail.
#define NS_E_REG_QUERYVALUE_FAIL							   MAKE_NSHRESULTERROR (0x8141)  // Query regitry value fail.
#define NS_E_REG_OPENKEY_FAIL								   MAKE_NSHRESULTERROR (0x8142)  // Open regitry key fail.
#define NS_E_FIRST_FIELD_NOT_RECEIVED		    			   MAKE_NSHRESULTERROR (0x8143)  // First field has not been received yet.
#define NS_E_OPEN_1394DEVICE_FAIL		    				   MAKE_NSHRESULTERROR (0x8144)  // Fail to open 1394 device.
#define NS_E_DEVICE_NOT_BEEN_OPENED		    				   MAKE_NSHRESULTERROR (0x8145)  // Fail to open 1394 device.

#define NS_E_NO_VALID_SEEK_COMMAND		    				   MAKE_NSHRESULTERROR (0x8146)  // No valid seek command.
#define NS_E_DIRECT3D_CREATION_FAILED	    				   MAKE_NSHRESULTERROR (0x8147)  // Failed to create direct3d 
#define NS_E_DIRECT3D_DEVICE_CREATION_FAILED	    		   MAKE_NSHRESULTERROR (0x8148)  // Failed to create direct3d device. 
#define NS_E_DIRECT3D_TEXTURE_SURFACE_CREATION_FAILED  		   MAKE_NSHRESULTERROR (0x8149)  // Failed to create direct3d texture surface.. 

#define NS_E_XVID_ERROR											MAKE_NSHRESULTERROR (0x814A)  // error returned by xvid

#define NS_E_INVALID_MPEG_STREAM								MAKE_NSHRESULTERROR (0x814B)  // Invalid mpeg stream

#define NS_E_StatusIONotSupported								MAKE_NSHRESULTERROR (0x814C)  // I/O is not supported

#define NS_E_INVALID_VIDEO_SIZE									MAKE_NSHRESULTERROR (0x814D)  // video size is invalid 
#define NS_E_UNSUPPORTED_VIDEO_STANDARD							MAKE_NSHRESULTERROR (0x814E)  // video size is invalid 
#define NS_E_INVALID_FILE										MAKE_NSHRESULTERROR (0x814F)  // invalid file
#define NS_E_END_OF_FILE										MAKE_NSHRESULTERROR (0x8150)  // end of file
#define NS_E_MPEG2_NO_SEQUENCE_EXTENSION						MAKE_NSHRESULTERROR (0x8151)  // mepg2 stream has no sequence extension
#define NS_E_MPEG_STREAM_INFO_NOT_FOUND							MAKE_NSHRESULTERROR (0x8152)  // Has not found the mpeg stream information
#define NS_E_NO_IOINFO                							MAKE_NSHRESULTERROR (0x8153)  // no IO info return from NetManager
#define NS_E_NOT_SUPPORTED_CG_FILE                				MAKE_NSHRESULTERROR (0x8154)  // not supported cg file
#define NS_E_INVALID_CG_FUNCTION                				MAKE_NSHRESULTERROR (0x8155)  // Invalid cg function loaded from dll
#define NS_E_CG_FUNCTION_FAILED                 				MAKE_NSHRESULTERROR (0x8156)  // failed when call cg function
#define NS_E_READ_DATA_INVALID                 					MAKE_NSHRESULTERROR (0x8157)  // read data invalid
#define NS_INVALID_VCR_COMMAND                 					MAKE_NSHRESULTERROR (0x8158)  // read data invalid
#define NS_INVALID_COM_PORT_ID                 					MAKE_NSHRESULTERROR (0x8159)  // read data invalid

#define NS_E_UPLOAD_PROJECT                                     MAKE_NSHRESULTERROR (0x8160)  // Generic error.
#define NS_E_INVALID_SAVE_PATH                                  MAKE_NSHRESULTERROR (0x8161)  // file path to save clip is invalid;
#define NS_E_MAKE_UNIQUE_CLIP_SAVE_NAME_FAILED                  MAKE_NSHRESULTERROR (0x8162)  // can't create a unique clip name;   
#define NS_E_NET_SOCKET_ERROR                                   MAKE_NSHRESULTERROR (0x8163)  // general socket error;
#define NS_E_NET_SOCKET_NOT_CONNECT                             MAKE_NSHRESULTERROR (0x8164)  // 
#define NS_E_NET_SOCKET_SHUT_DOWN                               MAKE_NSHRESULTERROR (0x8165)  // 
#define NS_E_PD_FTP_ALREADY_LOGED_IN                            MAKE_NSHRESULTERROR (0x8166)  // 
#define NS_E_PD_FTP_LOGED_FAILED_PASSWORD_INVALID               MAKE_NSHRESULTERROR (0x8167)  // 
#define NS_E_PD_FTP_LOGED_FAILED_NOT_NET_MODE                   MAKE_NSHRESULTERROR (0x8168)  // 
#define NS_E_PD_FTP_LOGED_FAILED_NOT_NO_MOUNTED_MEDIA           MAKE_NSHRESULTERROR (0x8169)  // 
#define NS_E_PD_FTP_LOGED_FAILED_REJECTED_FOR_CURRENT_STATE     MAKE_NSHRESULTERROR (0x816A)  // 
#define NS_E_PD_FTP_LOGED_FAILED_SESSION_UNDER_USE              MAKE_NSHRESULTERROR (0x816B)  // 
#define NS_E_PD_FTP_LOGED_FAILED_ALREADY_MAXIMUM_USERS          MAKE_NSHRESULTERROR (0x816C)  //          
#define NS_E_ALREADY_SHOT_DETECTION                             MAKE_NSHRESULTERROR (0x8172)  // Shot detection has already run!
#define NS_E_AUDIO_SAMPLERATE_NOT_SUPPORTED                     MAKE_NSHRESULTERROR (0x8173)  // Audio sample rate not supported!

#define NS_E_XDCAM_UPLOAD_READ_SRC_FILE_FAILED                  MAKE_NSHRESULTERROR (0x8174)  // read source file failed;
#define NS_E_XDCAM_UPLOAD_WRITE_DST_FILE_FAILED                 MAKE_NSHRESULTERROR (0x8175)  // write target file failed;
#define NS_E_XDCAM_UPLOAD_NO_ENOUGH_DISK_SPACE                  MAKE_NSHRESULTERROR (0x8176)  // no enough disk space;
#define NS_E_XDCAM_UPLOAD_RESAMPLE_AUDIO_FAILED                 MAKE_NSHRESULTERROR (0x8177)  // resample audio data failed;
#define NS_E_XDCAM_UPLOAD_CLIP_NOT_EXIST                        MAKE_NSHRESULTERROR (0x8178)  // clip not exist;
#define NS_E_XDCAM_UPLOAD_PD_NOT_MATCH                          MAKE_NSHRESULTERROR (0x8179)  // PD not match;
#define NS_E_XDCAM_UPLOAD_NET_CONNECT_FAILED                    MAKE_NSHRESULTERROR (0x817A)  // net connect failed;
#define NS_E_XDCAM_UPLOAD_USER_ABOR_TASK                        MAKE_NSHRESULTERROR (0x817B)  // user abor upload task;
#define NS_E_XDCAM_UPLOAD_CANNT_DETECT_PD_DRIVE                 MAKE_NSHRESULTERROR (0x817C)  // can not detect PD drive;
#define NS_E_UPLOAD_GET_TASK_FAILED                             MAKE_NSHRESULTERROR (0x817D)  // get upload task failed;
#define NS_E_PROJECT_FOOTAGE_OFFLINE                            MAKE_NSHRESULTERROR (0x817E)  // footage offline;
#define NS_E_MP4_GOP_COUNT_TOO_LARGE							MAKE_NSHRESULTERROR (0x817F)  // mp4 gop too large;
#define NS_E_FILMODE_DVD_NOT_SUPPORTED					        MAKE_NSHRESULTERROR (0x8180)  // FilmModeDVD.

#define NS_E_P2_UPLOAD_HDCLIP_LOW_STREAM                        MAKE_NSHRESULTERROR (0x8181)  // HD clip import low stream donot surport current;
#define NS_E_P2_UPLOAD_READ_SRC_FILE_FAILED                     MAKE_NSHRESULTERROR (0x8182)  // read source file failed;
#define NS_E_P2_UPLOAD_WRITE_DST_FILE_FAILED                    MAKE_NSHRESULTERROR (0x8183)  // write target file failed;
#define NS_E_P2_UPLOAD_NO_ENOUGH_DISK_SPACE                     MAKE_NSHRESULTERROR (0x8184)  // no enough disk space;
#define NS_E_P2_UPLOAD_USER_ABOR_TASK                           MAKE_NSHRESULTERROR (0x8185)  // user abor upload task;
#define NS_E_P2_UPLOAD_DECOMPRESS_SRC_FILE_FAILED               MAKE_NSHRESULTERROR (0x8186)  // decompress source file data failed;
#define NS_E_P2_UPLOAD_NOT_SET_SOURCE_FILE_INFO                 MAKE_NSHRESULTERROR (0x8187)  // do not set source file info;
#define NS_E_INVALID_CODEC_ID									MAKE_NSHRESULTERROR (0x8188)  // invalid NA codec ID;
#define NS_E_CANNOT_FIND_DECODER								MAKE_NSHRESULTERROR (0x8189)  // can not find codec ;
#define NS_E_OPEN_CODEC_FAIL									MAKE_NSHRESULTERROR (0x818A)  // can not open codec ;
#define NS_E_DECODE_FRAME_FAIL									MAKE_NSHRESULTERROR (0x818B)  // decode frame fail;
#define NS_E_ENCODE_FRAME_FAIL									MAKE_NSHRESULTERROR (0x818C)  // ENCODE frame fail;
#define NS_E_PSD_IMAGE_TOO_LARGE                                MAKE_NSHRESULTERROR (0x818D)  // Not support so large PSD image
#define NS_E_PSD_IMAGE_ZERO_PIXEL                               MAKE_NSHRESULTERROR (0x818E)  // Image pixel count is zero
#define NS_E_MEDIADEVICE_UPLOAD_CANNT_FIND_LOCAL_PATH           MAKE_NSHRESULTERROR (0x818F)  // can not find Device back-up path

// Media Device
#define NS_E_MEDIA_DEVICE_USER_ABOR_UPLOAD                      MAKE_NSHRESULTERROR (0x8190)  // user abor upload task;
#define NS_E_MEDIA_DEVICE_NO_DISK_SPACE                         MAKE_NSHRESULTERROR (0x8191)  // no enough disk space;
//

#define NS_E_INVALID_MOB_REFERENCE		                        MAKE_NSHRESULTERROR (0x8192)  // invalid mob reference;
#define NS_E_NOT_RENDERED_EFFECT		                        MAKE_NSHRESULTERROR (0x8193)  // not rendered effect;

// NTM.
#define NS_E_INVALID_INTERPLAY_WEBSERVICE_USER_OR_PASSWORD      MAKE_NSHRESULTERROR (0x8194)  // invalid username or password of Interplay WebSevice ;
#define NS_E_ISIS_STORAGE_PATH_NOT_FOUND                        MAKE_NSHRESULTERROR (0x8195)  // invalid ISIS Storage Path;
#define NS_E_INTERPLAY_URI_NOT_FOUND                            MAKE_NSHRESULTERROR (0x8196)  // invalid interplay URI;
#define NS_E_UPLOADING_META_FILE_NOT_FOUND                      MAKE_NSHRESULTERROR (0x8197)  // invalid uploading meta file;
#define NS_E_UPLOADING_META_FILE_FORMAT_NOT_SUPPORTED			MAKE_NSHRESULTERROR (0x8198)  // not supported format of meta file;
#define NS_E_UPLOADING_MEDIA_FILE_FORMAT_NOT_SUPPORTED          MAKE_NSHRESULTERROR (0x8199)  // not supported format of media file;
#define NS_E_UPLOADING_MEDIA_FILE_NOT_FOUND                     MAKE_NSHRESULTERROR (0x819A)  // not supported format of media file;
#define NS_E_DOWNLOADIND_LOCAL_STORAGE_PATH_NOT_FOUND           MAKE_NSHRESULTERROR (0x819B)  
#define NS_E_DOWNLOADIND_MEDIA_FILE_NOT_FOUND                   MAKE_NSHRESULTERROR (0x819C)  
#define NS_E_DOWNLOADIND_MEDIA_FILE_FORMAT_NOT_SUPPORTED        MAKE_NSHRESULTERROR (0x819D)  
#define NS_E_DOWNLOADIND_NOMEDIA					            MAKE_NSHRESULTERROR (0x819E) 
#define NS_E_DOWNLOADIND_AUDIOTRACK_OUTRANGE		            MAKE_NSHRESULTERROR (0x819F) 
#define NS_E_LOAD_CG_MODULE_DLL_FAILED							MAKE_NSHRESULTERROR (0x81A0) // load CG module dll Failed 
#define NS_E_NO_INVALID_SNG_OR_SSG_FILE_INPUT					MAKE_NSHRESULTERROR (0x81A1) // No invalid sng or ssg input for converting xml file 
#define NS_E_WRITE_CG_XML_FILE_FAILED							MAKE_NSHRESULTERROR (0x81A2) // error of writing xml file failed returned from CG module  
#define NS_E_EFFECT_IN_SEQUENCE									MAKE_NSHRESULTERROR (0x81A3) // some effects in sequence, maybe not supported  
#define NS_E_MULTY_TRACK_IN_SEQUENCE							MAKE_NSHRESULTERROR (0x81A4) // Multy track in sequence, maybe not supported

#define NS_E_NVS_NOT_CUED										MAKE_NSHRESULTERROR (0x81B4)

// GPU
#define NS_E_GPU_ENGINE_CREATION_FAILED	    					MAKE_NSHRESULTERROR (0x81C1)  // Failed to create GPU Engine 
#define NS_E_GPU_ENGINE_INITIALIZE_FAILED	    				MAKE_NSHRESULTERROR (0x81C2)  // Failed to initialize GPU Engine. 
#define NS_E_GPU_ENGINE_TEXTURE_SURFACE_CREATION_FAILED  		MAKE_NSHRESULTERROR (0x81C3)  // Failed to create GPU Engine texture surface.. 
#define NS_E_GPU_DEVICELOST										MAKE_NSHRESULTERROR (0x81C4)  // GPU Device Lost
#define NS_E_GPU_DEVICENOTRESET									MAKE_NSHRESULTERROR (0x81C5)  // GPU Not Reset
#define NS_E_GPU_OUTOFVIDEOMEMORY								MAKE_NSHRESULTERROR (0x81C6)  // GPU out of memory
#define NS_E_GPU_UNKNOWN										MAKE_NSHRESULTERROR (0x81C7)  // GPU Reserved error code
#define NS_E_TOO_LARGE_DATA_SIZE								MAKE_NSHRESULTERROR (0x81C8)  // GOP Data Reserved error code

// Speech Recognition
#define NS_E_SR_HIMALAYA_ENVVAR_NOT_FOUND	    				MAKE_NSHRESULTERROR (0x8200)  // Failed to get Himalaya Environment Variable
#define NS_E_SR_CREATE_WAV_2_MFCC_PROCESS_FAILED                MAKE_NSHRESULTERROR (0x8201)  // Create WAV to MFCC process failed
#define NS_E_SR_CREATE_MFCC_2_REC_PROCESS_FAILED                MAKE_NSHRESULTERROR (0x8202)  // Create MFCC to REC process failed
#define NS_E_SR_CREATE_MATCH_KEYWORD_PROCESS_FAILED             MAKE_NSHRESULTERROR (0x8203)  // Create match key word process failed
#define NS_E_SR_CREATE_CACHE_PATH_FAILED                        MAKE_NSHRESULTERROR (0x8204)  // Create cache path failed
#define NS_E_SR_CREATE_WORKING_PATH_FAILED                      MAKE_NSHRESULTERROR (0x8205)  // Create working path failed
#define NS_E_SR_CONFIG_FILE_NOT_EXIST                           MAKE_NSHRESULTERROR (0x8206)  // Config file not exist
#define NS_E_SR_DIC_FILE_NOT_EXIST                              MAKE_NSHRESULTERROR (0x8207)  // DIC file not exist
#define NS_E_SR_GRAMMA_FILE_NOT_EXIST                           MAKE_NSHRESULTERROR (0x8208)  // GRAMMA file not exist
#define NS_E_SR_MACROS_FILE_NOT_EXIST                           MAKE_NSHRESULTERROR (0x8209)  // Macros file not exist
#define NS_E_SR_MODEL_FILE_NOT_EXIST                            MAKE_NSHRESULTERROR (0x820A)  // Model file not exist
#define NS_E_SR_NET_FILE_NOT_EXIST                              MAKE_NSHRESULTERROR (0x820B)  // Net file not exist
#define NS_E_SR_SYLLABLE_FILE_NOT_EXIST                         MAKE_NSHRESULTERROR (0x820C)  // Syllable file not exist
#define NS_E_SR_WORKING_PATH_DISK_SPACE_SMALL                   MAKE_NSHRESULTERROR (0x820D)  // Syllable file not exist

#define NS_E_INVALID_VAIX                                       MAKE_NSHRESULTERROR (0x820E)  // The vaix file is not supported.
#define NS_E_HAS_NOT_FOUND_DOG                                  MAKE_NSHRESULTERROR (0x820F)  // Has not found dog.
									
#define PX_E_CSP_FILE_FORMAT_INVALID							MAKE_NSHRESULTERROR (0x8210)  // Invalid course package file format.

// ASF
#define PX_E_ASF_INVALID_PAYLOAD							    MAKE_NSHRESULTERROR (0x8211)  // Invalid payload.
#define PX_E_ASF_BUFFER_AND_FILE_SPACE_NOT_INIT                 MAKE_NSHRESULTERROR (0x8212)  // buffer and file space not init.

//PxDeviceHK
#define PX_E_PXDEVICEHK_INIT_ERROR                              MAKE_NSHRESULTERROR (0x8213)  // hk device init fail.
#define PX_E_PXDEVICEHK_LOGIN_ERROR                             MAKE_NSHRESULTERROR (0x8214)  // hk device login fail.
#define PX_E_PXDEVICEHK_REALPLAY_ERROR                          MAKE_NSHRESULTERROR (0x8215)  // hk device realplay fail.
#define PX_E_PXDEVICEHK_GETDVRCONFIG_ERROR                      MAKE_NSHRESULTERROR (0x8216)  // hk device GetDVRConfig fail.
#define PX_E_PXDEVICEHK_MAKEKEYFRAME_ERROR                      MAKE_NSHRESULTERROR (0x8217)  // hk device makeKeyFrame fail
#define PX_E_PXDEVICEHK_CREATESTREAMEX_ERROR                    MAKE_NSHRESULTERROR (0x8218)  // hk device createstreamex fail
#define PX_E_PXDEVICEHK_INPUTDATA_ERROR                         MAKE_NSHRESULTERROR (0x8219)  // hk device inputdata fail
#define PX_E_PXDEVICEHK_PACKETINFOEX_PARA_ERROR                 MAKE_NSHRESULTERROR (0x8220) // hk device inputdata fail
#define PX_E_INVALID_REALPLAYID                                 MAKE_NSHRESULTERROR (0x8221)  // hk invalid realplayid
#define PX_E_PXDEVICEHK_READDARACALLBACK_ERROR                  MAKE_NSHRESULTERROR (0x8222)  // hk device readdatacallback fail
#define PX_E_PXDEVICEHK_SETREALDATACALLBACK_ERROR               MAKE_NSHRESULTERROR (0x8223)  // hk device setrealdatacallback fail
#define PX_E_PXDEVICEHK_GETDEVICEPARA_ERROR                     MAKE_NSHRESULTERROR (0x8224)  // hk device getdevicepara fail
#define PX_E_PXDEVICEHK_SETRECONNETTIME_ERROR                   MAKE_NSHRESULTERROR (0x8225)  // hk device setreconnettime fail
#define PX_E_PXDEVICEHK_GETRECONNETTIME_ERROR                   MAKE_NSHRESULTERROR (0x8226)  // hk device getreconnettime fail
#define PX_E_PXDEVICEHK_CLOSE_ERROR                             MAKE_NSHRESULTERROR (0x8227)  // hk device close fail
#define PX_E_PXDEVICEHK_STOPREALPLAY_ERROR                      MAKE_NSHRESULTERROR (0x8228)  // hk device stoprealpaly fail
#define PX_E_PXDEVICEHK_LOGOUT_ERROR                            MAKE_NSHRESULTERROR (0x8229)  // hk device logout fail     
#define PX_E_INVALID_IN_LID                                     MAKE_NSHRESULTERROR (0x8230)  // hk in_lID invalid
#define PX_E_INVALID_IN_PSDEVICECLIENT                          MAKE_NSHRESULTERROR (0x8231)  // hk in_psDeviceClient invalid
#define PX_E_INVALID_IN_LTIME                                   MAKE_NSHRESULTERROR (0x8232)  // hk in_lTime invalid
#define PX_E_WAIT_TIMEOUT                                       MAKE_NSHRESULTERROR (0x8233)  // hk wait for video or audio time out
#define PX_E_WAIT_OTHERFAIL                                     MAKE_NSHRESULTERROR (0x8234)  // hk wait for video or audio other error
#define PX_E_PXDEVICEHK_DUPLICATIONDEVICE                       MAKE_NSHRESULTERROR (0x8235)  // hk add device is exist
#define PX_E_PXDEVICEHK_INVALID_AUDIO_ENCODETYPE                MAKE_NSHRESULTERROR (0x8236)  // hk Invalid audio encode type

//PxFLVRecorder
#define PX_E_PXFLVRECORDER_CREATEFILE_ERROR                     MAKE_NSHRESULTERROR (0x8236)  // flv recorder create file error  
#define PX_E_PXFLVRECORDER_ADDFILE_ERROR                        MAKE_NSHRESULTERROR (0x8237)  // flv recorder add file error
#define PX_E_PXFLVRECORDER_SETFILEPROPERTY_ERROR                MAKE_NSHRESULTERROR (0x8238)  // flv recorder set file property error
#define PX_E_PXFLVRECORDER_BEGINWRITRFILR_ERROR                 MAKE_NSHRESULTERROR (0x8239)  // flv recorder begin write file error
#define PX_E_PXFLVRECORDER_WRITEVIDEOSAMPLE_ERROR               MAKE_NSHRESULTERROR (0x8240)  // flv recorder write video sample error
#define PX_E_PXFLVRECORDER_WRITEAUDIOSAMPLE_ERROR               MAKE_NSHRESULTERROR (0x8241)  // flv recorder write audio sample error
#define PX_E_PXFLVRECORDER_ENDWRITEFILE_ERROR                   MAKE_NSHRESULTERROR (0x8242)  // flv recorder end write file error
#define PX_E_PXFLVRECORDER_DELFILE_ERROR                        MAKE_NSHRESULTERROR (0x8243)  // flv recorder del file error
#define PX_E_PXFLVRECORDER_WRITEFILE_ERROR                      MAKE_NSHRESULTERROR (0x8244)  // flv recorder write buffer error
#define PX_E_PXFLVRECORDER_GETAVCNALTYPE_ERROR                  MAKE_NSHRESULTERROR (0x8245)  // flv recorder get avc nal type error
#define PX_E_PXFLVRECORDER_GETBUFFERFRAMETYPE_ERROR             MAKE_NSHRESULTERROR (0x8246)  // flv recorder get buffer frame type error

//PxDeviceMpeg264
#define PX_E_PXDEVICEMPEG264_EXCEPTION_ERROR                        MAKE_NSHRESULTERROR (0x8247)

//PxRecordControl
#define PX_E_PXRECORDCONTROL_CHANGEFILE_TOO_FAST                        MAKE_NSHRESULTERROR (0x8248)

#define NS_E_SERVER_AUTHORIZATION_FAIL							MAKE_NSHRESULTERROR (0x8249)
//StreamDataServer Recorder
#define NS_E_STREAM_NOT_RECORDING								MAKE_NSHRESULTERROR (0x8250)
#define NS_E_INVALID_ADDRESS									MAKE_NSHRESULTERROR (0x8251)
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ---------------------- WARNINGS CODE ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#define  NS_W_HW_OVERLAY_NOT_AVAILABLE                  MAKE_NSHRESULTWARNING(0x0001)  // Hardware overlay for live window display is not available.
#define  NS_W_NOT_SUPPORTED                             MAKE_NSHRESULTWARNING(0x0002)  // The operation was not completed because it is not supported, but it does not harm the system.
#define  NS_W_BITMAP_NEED_UPDATE                        MAKE_NSHRESULTWARNING(0x0003)  // Information that tells that a bitmap update is required for the live window.
#define  NS_W_PREVIOUS_NODE_REPLACED                    MAKE_NSHRESULTWARNING(0x0004)  // Information that tells that an existing tree node has been replaced.
#define  NS_W_MEMORY_WAS_FREE                           MAKE_NSHRESULTWARNING(0x0005)  // Information that tells that memory was free with the resource manager garbage collection.
#define  NS_W_MEMORY_NO_ACTION                          MAKE_NSHRESULTWARNING(0x0006)  // Information that tells that resource manager was not able to free any memory on garbage collection.
#define  NS_W_NOT_EXECUTING                             MAKE_NSHRESULTWARNING(0x0007)  // Information cannot be accessed because the job is not executing.
#define  NS_W_ITEM_FOUND                                MAKE_NSHRESULTWARNING(0x0008)  // Information that tells that the specified item was found.
#define  NS_W_EMPTY_ENGINE                              MAKE_NSHRESULTWARNING(0x0009)  // Information that tells that returned values may not be good because the processing engine is empty.
#define  NS_W_ENUM_END_OF_LIST                          MAKE_NSHRESULTWARNING(0x000A)  // Enumerator has reached the end of the list to enumerate.
#define  NS_W_FIELD_NOT_FOUND_IN_FILE                   MAKE_NSHRESULTWARNING(0x000B)  // No specified field name is found in the file at the current position.
#define  NS_W_POS_AT_BOUNDARY                           MAKE_NSHRESULTWARNING(0x000C)  // The timeline position is at the boundary of an element.
#define  NS_W_POS_NOT_IN_ELEMENT                        MAKE_NSHRESULTWARNING(0x000D)  // The timeline posiiton is not inside any eleemnt. It is inside a hole.
#define  NS_W_EMPTY_RANGE                               MAKE_NSHRESULTWARNING(0x000E)  // The start point and the end point of the specified range are equal.
#define  NS_W_NOTHING_TO_DO                             MAKE_NSHRESULTWARNING(0x000F)  // The object doesn't have anything else to do.
#define  NS_W_TASK_NEW_POSITION                      MAKE_NSHRESULTWARNING(0x0010)  // The object tasks a new position in order to be fully efficient.
#define  NS_W_MEMORY_NEEDED                             MAKE_NSHRESULTWARNING(0x0011)  // The object needs memory in order to continue doing its work.
#define  NS_W_CONTAINER_EMPTY                           MAKE_NSHRESULTWARNING(0x0012)  // The container was empty on the call.
#define  NS_W_CONTAINER_NOW_EMPTY                       MAKE_NSHRESULTWARNING(0x0013)  // The container was emptied by the the call.
#define  NS_W_ITEM_FOUND_NOT_COMPLETED                  MAKE_NSHRESULTWARNING(0x0014)  // Information that tells that the specified item was found but operation is not completed.
#define  NS_W_WRONG_EVENT_STATE                         MAKE_NSHRESULTWARNING(0x0015)  // An event is not in the state that should be (signalled or unsignalled).
#define  NS_W_LATE_BUFFER_DROPPED_AT_OUTPUT             MAKE_NSHRESULTWARNING(0x0016)  // A buffer arrived late at the output and was dropped.
#define  NS_W_STOP_TASKED                            MAKE_NSHRESULTWARNING(0x0017)  // While doing some processing, a stop command arrived.
#define  NS_W_FILE_NOT_FOUND                            MAKE_NSHRESULTWARNING(0x0018)  // The file was not found.
#define  NS_W_DROPPED_CAPTURE_OF_VIDEO_AT_INPUT         MAKE_NSHRESULTWARNING(0x0019)  // Dropped some video at capture of input.
#define  NS_W_DROPPED_CAPTURE_OF_AUDIO_AT_INPUT         MAKE_NSHRESULTWARNING(0x001A)  // Dropped some audio at capture of input.
#define  NS_W_MEMORY_WAS_FREE_AUDIO                     MAKE_NSHRESULTWARNING(0x0020)  // Information that tells that audio memory was free with the resource manager garbage collection.
#define  NS_W_MEMORY_WAS_FREE_VIDEO                     MAKE_NSHRESULTWARNING(0x0021)  // Information that tells that video memory was free with the resource manager garbage collection.
#define  NS_W_MEMORY_WAS_FREE_AUDIO_AND_VIDEO           MAKE_NSHRESULTWARNING(0x0022)  // Information that tells that audio and video memory was free with the resource manager garbage collection.
#define  NS_W_CAPTURE_OF_VIDEO_AT_INPUT_START_LATE      MAKE_NSHRESULTWARNING(0x0023)  // Start Time set for first video surface to be captured from input is late.
#define  NS_W_CAPTURE_OF_AUDIO_AT_INPUT_START_LATE      MAKE_NSHRESULTWARNING(0x0024)  // Start Time set for first audio buffer to be captured from input is late.
#define  NS_W_CLOSE_TO_END_OF_DISK                      MAKE_NSHRESULTWARNING(0x0025)  // close to end of disk .
#define  NS_W_AVI_AUDIO_NOT_SUPPORTED                   MAKE_NSHRESULTWARNING(0x0026)  // The interleaved AV file info is known to matrox file reader but the audio format is not supported. The video portion of the file may still be played.
#define  NS_W_AVI_VIDEO_NOT_SUPPORTED                   MAKE_NSHRESULTWARNING(0x0027)  // The interleaved AV file info is known to matrox file reader but the video format is not supported. The audio portion of the file may still be played.
#define  NS_W_MISMATCH_PIXEL_ASPECT_RATIO               MAKE_NSHRESULTWARNING(0x0028)  // The pixel aspect ratio is not the same as the current pixel aspect ratio determined from the editing resolution. The file can still be streamed.
#define  NS_W_MISMATCH_FILE_ASPECT_RATIO                MAKE_NSHRESULTWARNING(0x0029)  // The file aspect ratio is not the same as the current editing aspect ratio. The file may still be streamed.
#define  NS_W_LOST_EMBEDDED_AUDIO_INPUT                 MAKE_NSHRESULTWARNING(0x002A)  // The embedded audio input is lost in SDBOB. 
#define  NS_W_LOST_VIDEO_INPUT                          MAKE_NSHRESULTWARNING(0x002B)  // The video input is lost in SDBOB.  
#define  NS_W_LOST_GENLOCK                              MAKE_NSHRESULTWARNING(0x002C)  // The genlock was lost on SDBOB.
#define  NS_W_GENLOCK_CHANGED                           MAKE_NSHRESULTWARNING(0x002D)  // The genlock source has changed.
#define  NS_W_MISMATCH_SCAN_MODE                        MAKE_NSHRESULTWARNING(0x002E)  // The scan mode of the file and the editing resolution are different.
#define  NS_W_AUDIO_INPUT_ABSENT                        MAKE_NSHRESULTWARNING(0x002F)  // The audio input is not present.
#define  NS_W_UIF_VIDEO_BUFFER_NOT_CAPTURED             MAKE_NSHRESULTWARNING(0x0030)  // The UIF video capture returned a video buffer that was not captured into.
#define  NS_W_UIF_AUDIO_BUFFER_NOT_CAPTURED             MAKE_NSHRESULTWARNING(0x0031)  // The UIF audio capture returned an audio buffer that was not captured into.
#define  NS_W_AUDIO_DROPPED_AT_RENDERING                MAKE_NSHRESULTWARNING(0x0032)  // An audio buffer has been asked to be dropped during rendering.
#define  NS_W_VIDEO_DROPPED_AT_RENDERING                MAKE_NSHRESULTWARNING(0x0033)  // A video buffer has been asked to be dropped during rendering.
#define  NS_W_1394_INPUT_NOT_RECEIVING_DATA             MAKE_NSHRESULTWARNING(0x0034)  // The 1394 input is not receiving data.
#define  NS_W_NEWAUTO_1394_INPUT_DISCONNECTED           MAKE_NSHRESULTWARNING(0x0035)  // The 1394 input is disconnected.
#define  NS_W_AUDIO_INPUT_START_SILENCE                     MAKE_NSHRESULTWARNING(0x0036)  // The audio input started generating silence
#define  NS_W_VIDEO_INPUT_START_BLACK                       MAKE_NSHRESULTWARNING(0x0037)  // The video input started generating black
#define  NS_W_AUDIO_INPUT_GLITCH                            MAKE_NSHRESULTWARNING(0x0038)  // The audio input generated some silence because there was input problem
#define  NS_W_VIDEO_INPUT_GLITCH                            MAKE_NSHRESULTWARNING(0x0039)  // The video input generated some black because there was input problem
#define  NS_W_1394_INPUT_START_RESOLUTION_MISMATCH          MAKE_NSHRESULTWARNING(0x003A)  // The 1394 input started receiving data with wrong resolution.
#define  NS_W_WAV_FILE_SIZE_LIMIT_ALMOST_REACHED            MAKE_NSHRESULTWARNING(0x003B)  // Close to .wav file size limit of 4 GB
#define  NS_W_END_OF_PIPE_FOUND                             MAKE_NSHRESULTWARNING(0x003C)  // An EndOfPipe element was found, need to handle this
#define  NS_W_TASK_CANCEL_NOT_SUPPORTED                  MAKE_NSHRESULTWARNING(0x003D)  // A task cannot be cancelled because it is not supported by the task properties.
#define  NS_W_CANNOT_CANCEL_ALREADY_FLUSHED                 MAKE_NSHRESULTWARNING(0x003E)  // A task cannot be cancelled because it is already completed and flushed.
#define  NS_W_INPUT_START_FRAME_RATE_MISMATCH               MAKE_NSHRESULTWARNING(0x003F)  // The input started receiving data with wrong frame rate.
#define  NS_W_NO_MEMORYCOLLECTORSET_FOR_GIVEN_POS_AND_DUR   MAKE_NSHRESULTWARNING(0x0040)  // A ProcessAll or ProcessBest pipe din't have a MemoryCollectorSet to put the given MemoryCollector.
#define  NS_W_TASK_COMPLETED                                MAKE_NSHRESULTWARNING(0x0041)  // Tells the task execution manager that the last task executed is now completed.
#define  NS_W_PAUSE_AT_TIME_INTERMEDIATE                    MAKE_NSHRESULTWARNING(0x0042)  // Tells the trigger thread that the pause at position returned is not valid yet.
#define  NS_W_RUN_AT_TIME_INTERMEDIATE                      MAKE_NSHRESULTWARNING(0x0043)  // Tells the trigger thread that the run at position returned is not valid yet.
#define  NS_W_STILL_IN_USE                                  MAKE_NSHRESULTWARNING(0x0044)  // Protects access of elements in the VideoCache
#define  NS_W_MTX_1394_INPUT_DISCONNECTED					MAKE_NSHRESULTWARNING(0x0045)  // The Matrox 1394 input is disconnected.

#define  NS_W_SOURCESWITCH_2PLAY					MAKE_NSHRESULTWARNING(0x0046)
#define  NS_W_SOURCESWITCH_PLAYING					MAKE_NSHRESULTWARNING(0x0047)
#define  NS_W_SOURCESWITCH_2PAUSE					MAKE_NSHRESULTWARNING(0x0048)
#define  NS_W_SOURCESWITCH_PAUSING					MAKE_NSHRESULTWARNING(0x0049)
#define  NS_W_SOURCESWITCH_2SEEK					MAKE_NSHRESULTWARNING(0x004A)
#define  NS_W_SOURCESWITCH_2STOP					MAKE_NSHRESULTWARNING(0x004B)
#define  NS_W_SOURCESWITCH_2SWITCH					MAKE_NSHRESULTWARNING(0x004C)

#define  NS_W_COMMAND_NOT_COMPLETED					MAKE_NSHRESULTWARNING(0x004D)
#define  NS_W_SIGNAL_NOT_PRESENT                    MAKE_NSHRESULTWARNING(0x004E)



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ---------------------- MESSAGE CODES ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define  NS_M_EC_COMPLETE                               MAKE_NSHRESULTWARNING(0x2000)  // EC_COMPLETE
#define  NS_M_FILE_CLOSED                               MAKE_NSHRESULTWARNING(0x2001)  // The file has been closed. Relevent for capture.
#define  NS_M_FILE_SWITCHED                             MAKE_NSHRESULTWARNING(0x2002)  // The file has been switched. Notification given by SinkSwitcher. 
#define  NS_M_START_REPEATING                           MAKE_NSHRESULTWARNING(0x2003)  // The video output has started to repeat. 
#define  NS_M_END_REPEATING                             MAKE_NSHRESULTWARNING(0x2004)  // The video output has stopped to repeat. 
#define  NS_M_START_SILENCE                             MAKE_NSHRESULTWARNING(0x2005)  // The audio output has started to play silence. 
#define  NS_M_END_SILENCE                               MAKE_NSHRESULTWARNING(0x2006)  // The audio output has stopped to play silence. 
#define  NS_M_GRAPH_HAS_BEEN_STOPPED                    MAKE_NSHRESULTWARNING(0x2007)  // The filter graph has been stopped due to a fatal error happens
#define  NS_M_VIDEO_INPUT_RECONNECTED                   MAKE_NSHRESULTWARNING(0x2008)  // The video input gets reconnected 
#define  NS_M_EMBEDDED_AUDIO_INPUT_RECONNECTED          MAKE_NSHRESULTWARNING(0x2009)  // The embedded audio input get reconnected.
#define  NS_M_PERFORMANCE_FACTOR_REPORTED               MAKE_NSHRESULTWARNING(0x200A)  // A performance factor has been reported.
#define  NS_M_EC_COMPLETE_FOR_STOP                      MAKE_NSHRESULTWARNING(0x200B)  // EC_COMPLETE_FOR_STOP
#define  NS_M_FILE_CAPTURE_FINISHED                     MAKE_NSHRESULTWARNING(0x200C)  // File has finished capturing to end-point
#define  NS_M_1394_INPUT_RECEIVING_DATA                 MAKE_NSHRESULTWARNING(0x200D)  // The 1394 has started to receive data.
#define  NS_M_1394_INPUT_CONNECTED                      MAKE_NSHRESULTWARNING(0x200E)  // The 1394 input is connected.
#define  NS_M_ELEMENT_READER_DATA_ENOUGH				MAKE_NSHRESULTWARNING(0x200F)
#define  NS_M_FILE_CAPTURE_BEGIN_REALLY					MAKE_NSHRESULTWARNING(0x2010)	// the capture really begin now
#define  NS_M_VTR_DISCONNECTED							MAKE_NSHRESULTWARNING(0x2011)  // The VTR is disconnected.
#define  NS_M_VTR_ENDOFTAPE								MAKE_NSHRESULTWARNING(0x2012)  // The tape of VTR is ended.
#define  NS_M_VTR_TIMECODE_DISCONTINUOUS				MAKE_NSHRESULTWARNING(0x2013)  // The timecode of VTR is not continual.
#define  NS_M_NEED_NEXT_PCM_FRAME                       MAKE_NSHRESULTWARNING(0x2014)  // Needing Read Next PCM Frame.
#define  NS_M_ALREADY_SHOT_DETECTION                    MAKE_NSHRESULTWARNING(0x2015)  // Shot detection has already run!
#define  NS_M_VTR_RECONNECTED							MAKE_NSHRESULTWARNING(0x2016)  // The VTR is Reconnected.
#define  NS_M_ALREADY_CG_DETECTION                      MAKE_NSHRESULTWARNING(0x2017)  // CG detection has already run!
#define  NS_M_FILE_RELEASE								MAKE_NSHRESULTWARNING(0x2018)  // File has finished capturing to end-point

#define  NS_M_INPUT_VIDEO_CHANGEED						MAKE_NSHRESULTWARNING(0x20FF)  // input video signal change

#define NS_W_INPUTSIGNAL__RESOLUTION_MISMATCH           MAKE_NSHRESULTWARNING(0x3000)   // input  signal change
//////////////////////////////////////////////////////////////////////////
// CG Error
//////////////////////////////////////////////////////////////////////////

#define NSD_E_DSX_MGFCRAWL_CREATE_INVALID				MAKE_NSHRESULTWARNING (0x9001)  // Create an invalid MGF crawling file.
#define NSD_E_DSX_INVALID_PARAMETERS					MAKE_NSHRESULTWARNING (0x9002)  // Invalid parameters.
#define NSD_E_DSX_MGFCRAWLWRITER_PAGEINFO_SIZE			MAKE_NSHRESULTWARNING (0x9003)  // MGF crawl writer: page size is wrong.
#define NSD_E_DSX_MGFCRAWLWRITER_WRITEFILE_FAIL			MAKE_NSHRESULTWARNING (0x9004)  // MGF crawl writer: write file failed.
#define NSD_E_DSX_CREATEFILE_FAIL						MAKE_NSHRESULTWARNING (0x9005)  // MGF crawl writer: create file failed.
#define NSD_E_DSX_SETFILEPOINTEREX_FAIL					MAKE_NSHRESULTWARNING (0x9006)  // MGF crawl writer: set file pointer extend failed.


#define NSD_E_DSX_MEDIAREADERNOTEXIST					MAKE_NSHRESULTWARNING (0x9007)  // Media File Reader do not find.
#define NSD_E_DSX_GETVIDEOFRAME_FAIL					MAKE_NSHRESULTWARNING (0x9008)  // Media File Reader Get Video Frame FAiled.
#define NSD_E_DSX_GETMEDIAFILEINFO					    MAKE_NSHRESULTWARNING (0x9009)  // Media File Reader Get Media File Info Failed.

#define NSD_E_DSX_SCROLLING_TOOMANYFILEDS				MAKE_NSHRESULTWARNING (0x9100)  // Too many fields in one read task.
#define NSD_E_DSX_SCROLLING_GROUP_EMPTY					MAKE_NSHRESULTWARNING (0x9101)  // Scrolling group list is empty.
#define NSD_E_DSX_SCROLLING_GROUP_NOT_FOUND				MAKE_NSHRESULTWARNING (0x9102)  // Can't find the group who issue the current I/O command.
#define NSD_E_DSX_CRAWLING_TOOMANYFILEDS				MAKE_NSHRESULTWARNING (0x9103)  // Too many fields in one read task.
#define NSD_E_DSX_CRAWLING_PAIR_EMPTY					MAKE_NSHRESULTWARNING (0x9104)  // Scrolling group list is empty.
#define NSD_E_DSX_CRAWLING_PAIR_NOT_FOUND				MAKE_NSHRESULTWARNING (0x9105)  // Can't find the group who issue the current I/O command.

#define NSD_E_DSX_RECYCLE_READER_FAILED 		        MAKE_NSHRESULTWARNING (0x9200)  //Recycle Reader Failed
#define NSD_E_DSX_ALLOC_BUFFER_FAILED 		            MAKE_NSHRESULTWARNING (0x9201)  //Can not Alloc Buffer


#define NS_E_VIDEO_EFFECT_NOT_AVAILABLE                 MAKE_NSHRESULTERROR (0x9300)  // The required video Effect is not available. This error code is returned for video only file. This file can not be played.
#define NS_E_AUDIO_EFFECT_NOT_AVAILABLE                 MAKE_NSHRESULTERROR (0x9301)  // The required audio Effect is not available. This error code is returned for audio only file. This file can not be played.

#define NS_E_STREAMCUTTER_NOTSUPPORT_NOAUDIO			MAKE_NSHRESULTERROR (0x9400)  //The stream media file has no audio
#define NS_E_STREAMCUTTER_NOTSUPPORT_NOVIDEO			MAKE_NSHRESULTERROR (0x9401)  //The stream media file has no video
#define NS_E_STREAMCUTTER_NOTSUPPORT_H264MUTISLICE      MAKE_NSHRESULTERROR (0x9402)  //The h264-coded stream media file has multiple slices in a frame or field
#define NS_E_STREAMCUTTER_NOTSUPPORT_H264INTERLACED     MAKE_NSHRESULTERROR (0x9403)  //The h264-coded stream media file is encoded in field
#define NS_E_STREAMCUTTER_NOTSUPPORT_NOTTS				MAKE_NSHRESULTERROR (0x9404)  //The mpg-series stream media file is not a ts file 
#define NS_E_INTERLEAVE_NOTSUPPORT_LARGEFILEREGION		MAKE_NSHRESULTERROR (0x9405)  //The video and audio data belonging to one frame spans too large a file region
#define NS_E_INTERLEAVE_NOTSUPPORT_FRAMERATE			MAKE_NSHRESULTERROR (0x9406)  //Frame rate other than 25fps is not support yet

#define NS_E_NOT_REMOTE_DATA_YET						MAKE_NSHRESULTERROR (0x9407)  //This LiveStream source has not new data from remote.
#define NS_E_INCOMPLETE_DATA							MAKE_NSHRESULTERROR (0x9408)  //There are still some datas left to receive.
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ------------------------ WARNIING CODES ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define NSD_W_DSX_CRAWLING_MEDIACHUNK_NOT_FOUND			MAKE_NSDHRESULTWARNING (0x8100)  // Can't find the media chunk for one page

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// ------------------------ COMMON ERROR CODES(not HRESULT) ----------------------------- //
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#define NS_ENOERR				0
#define NS_EINVLPARM			(-1)
#define NS_EINVLPTR				(-2)
#define NS_ENOMEM				(-3)
#define NS_EINVLIDX				(-4)


