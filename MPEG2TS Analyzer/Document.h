//
//  Document.h
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/04.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "tsparser.h"

@interface Document : NSDocument {
	NSInteger num_of_ts_packets_;
	NSData *data_;
}
@property (readonly) NSInteger num_of_ts_packets_;

- (void)getTSPacket:(NSInteger)index packet:(TSPacket *)packet;

@end
