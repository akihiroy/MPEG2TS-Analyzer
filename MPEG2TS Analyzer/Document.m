//
//  Document.m
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/04.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#import "Document.h"

@implementation Document

@synthesize num_of_ts_packets_;

- (id)init
{
    self = [super init];
    if (self) {
		// Add your subclass-specific initialization here.
		// If an error occurs here, return nil.
    }
    return self;
}

- (void)getTSPacket:(NSInteger)index packet:(TSPacket *)packet
{
	[data_ getBytes:packet->data range:NSMakeRange(index * 192 + 4, 188)];
}


- (NSString *)windowNibName
{
	// Override returning the nib file name of the document
	// If you need to use a subclass of NSWindowController or if your document supports multiple NSWindowControllers, you should remove this method and override -makeWindowControllers instead.
	return @"Document";
}

- (void)windowControllerDidLoadNib:(NSWindowController *)aController
{
	[super windowControllerDidLoadNib:aController];
	// Add any code here that needs to be executed once the windowController has loaded the document's window.
}

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
	/*
	 Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error when returning nil.
	You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
	*/
	NSException *exception = [NSException exceptionWithName:@"UnimplementedMethod" reason:[NSString stringWithFormat:@"%@ is unimplemented", NSStringFromSelector(_cmd)] userInfo:nil];
	@throw exception;
	return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
	num_of_ts_packets_ = [data length] / 192;
	data_ = data;
	return YES;
}

+ (BOOL)autosavesInPlace
{
    return YES;
}



@end
