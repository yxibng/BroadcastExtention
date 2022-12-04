//
//  TSConstants.h
//  TSPasS_iOS
//
//  Created by yxibng on 2020/11/4.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>
#elif TARGET_OS_OSX
#import <AppKit/AppKit.h>
#endif

extern CGSize const TSVideoDimension120x120;
/** 160 * 120
 */
extern CGSize const TSVideoDimension160x120;
/** 180 * 180
 */
extern CGSize const TSVideoDimension180x180;
/** 240 * 180
 */
extern CGSize const TSVideoDimension240x180;
/** 320 * 180
 */
extern CGSize const TSVideoDimension320x180;
/** 240 * 240
 */
extern CGSize const TSVideoDimension240x240;
/** 320 * 240
 */
extern CGSize const TSVideoDimension320x240;
/** 424 * 240
 */
extern CGSize const TSVideoDimension424x240;
/** 360 * 360
 */
extern CGSize const TSVideoDimension360x360;
/** 480 * 360
 */
extern CGSize const TSVideoDimension480x360;
/** 640 * 360
 */
extern CGSize const TSVideoDimension640x360;
/** 480 * 480
 */
extern CGSize const TSVideoDimension480x480;
/** 640 * 480
 */
extern CGSize const TSVideoDimension640x480;
/** 840 * 480
 */
extern CGSize const TSVideoDimension840x480;
/** 960 * 720 (Hardware dependent)
 */
extern CGSize const TSVideoDimension960x720;
/** 1280 * 720 (Hardware dependent)
 */
extern CGSize const TSVideoDimension1280x720;

/** 1920 * 1080 (Hardware dependent)
 */
extern CGSize const TSVideoDimension1920x1080;

#if TARGET_OS_OSX
/** 25400 * 1440 (Hardware dependent, macOS only)
 */
extern CGSize const TSVideoDimension2540x1440;
/** 3840 * 2160 (Hardware dependent, macOS only)
 */
extern CGSize const TSVideoDimension3840x2160;
#endif
