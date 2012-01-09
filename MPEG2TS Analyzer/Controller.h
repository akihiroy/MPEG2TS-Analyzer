//
//  Controller.h
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/05.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#import <AppKit/AppKit.h>

@interface Controller : NSObjectController {
	IBOutlet NSTextView *text_view_;
}
-(IBAction)selectTSPacket:(id)sender;
@end
