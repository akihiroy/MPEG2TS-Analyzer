//
//  Controller.m
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/05.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#import "Controller.h"
#import "Document.h"

@implementation Controller

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [_content num_of_ts_packets_];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
	if ([[tableColumn identifier] isEqualToString:@"number"]) {
		return [NSNumber numberWithInteger:row];
	} else {
		TSPacket packet;
		[_content getTSPacket:row packet:&packet];
		int pid = get_PID(&packet);
		if (pid == 0) {
			return @"PAT (0x0)";
		} else if (pid == 0x1FFF) {
			return @"Null (0x1FFF)";
		} else {
			return [NSString stringWithFormat:@"0x%X", pid];
		}
	}
}

- (BOOL)tableView:(NSTableView *)aTableView shouldSelectRow:(NSInteger)row
{
	return YES;
}

- (IBAction)selectTSPacket:(id)sender
{
	NSInteger selected_index = [sender selectedRow];
	TSPacket packet;
	[_content getTSPacket:selected_index packet:&packet];

	int adaptation_field_control = get_adaptation_field_control(&packet);
	
	NSMutableString *str = [[NSMutableString alloc] initWithString:@"Header\n"];
	[str appendFormat:@" sync_byte: 0x%X\n", get_sync_byte(&packet)];
	[str appendFormat:@" transport_error_indicator: %d\n", get_transport_error_indicator(&packet)];
	[str appendFormat:@" payload_unit_start_indicator: %d\n", get_payload_unit_start_indicator(&packet)];
	[str appendFormat:@" transport_priority: %d\n", get_transport_priority(&packet)];
	[str appendFormat:@" pid: %d\n", get_PID(&packet)];
	[str appendFormat:@" transport_scrambling_control: %d\n", get_transport_scrambling_control(&packet)];
	[str appendFormat:@" adaptation_field_control: %d\n", adaptation_field_control];
	[str appendFormat:@" continuity_counter: %d\n", get_continuity_counter(&packet)];

	if (adaptation_field_control == 2 || adaptation_field_control == 3) {
		[str appendFormat:@"\nAdaptation Field\n"];
		[str appendFormat:@" adaptation_field_length: %d\n", get_af_adaptation_field_length(&packet)];
		if (get_af_adaptation_field_length(&packet) > 0) {
			[str appendFormat:@" discontinuity_indicator: %d\n", get_af_discontinuity_indicator(&packet)];
			[str appendFormat:@" random_access_indicator: %d\n", get_af_random_access_indicator(&packet)];
			[str appendFormat:@" elementary_stream_priority_indicator: %d\n", get_af_elementary_stream_priority_indicator(&packet)];
			[str appendFormat:@" PCR_flag: %d\n", get_af_PCR_flag(&packet)];
			[str appendFormat:@" OPCR_flag: %d\n", get_af_OPCR_flag(&packet)];
			[str appendFormat:@" splicing_point_flag: %d\n", get_af_splicing_point_flag(&packet)];
			[str appendFormat:@" transport_private_data_flag: %d\n", get_af_transport_private_data_flag(&packet)];
			[str appendFormat:@" adaptation_field_extension_flag: %d\n", get_af_adaptation_field_extension_flag(&packet)];
			if (get_af_PCR_flag(&packet)) {
				[str appendFormat:@" program_clock_reference_base: %d\n", get_af_program_clock_reference_base(&packet)];
				[str appendFormat:@" program_clock_reference_extension: %d\n", get_af_program_clock_reference_extension(&packet)];
			}
			if (get_af_OPCR_flag(&packet)) {
				[str appendFormat:@" original_program_clock_reference_base: %d\n", get_af_original_program_clock_reference_base(&packet)];
				[str appendFormat:@" original_program_clock_reference_extension: %d\n", get_af_original_program_clock_reference_extension(&packet)];
			}
			if (get_af_splicing_point_flag(&packet)) {
				[str appendFormat:@" splice_countdown: %d\n", get_af_splice_countdown(&packet)];
			}
			if (get_af_transport_private_data_flag(&packet)) {
				int len = get_af_transport_private_data_length(&packet);
				[str appendFormat:@" transport_private_data_length: %d\n", len];
				[str appendString:@" transport_private_data:"];
				unsigned char *private_data = get_af_private_data(&packet);
				for (int i = 0; i < len; ++i) {
					[str appendFormat:@" %X", private_data[i]];
				}
			}
			if (get_af_adaptation_field_extension_flag(&packet)) {
				[str appendFormat:@" adaptation_field_extension_length: %d\n", get_af_adaptation_field_extension_length(&packet)];
				[str appendFormat:@" ltw_flag: %d\n", get_af_ltw_flag(&packet)];
				[str appendFormat:@" piecewise_rate_flag: %d\n", get_af_piecewise_rate_flag(&packet)];
				[str appendFormat:@" seamless_splice_flag: %d\n", get_af_seamless_splice_flag(&packet)];
				if (get_af_ltw_flag(&packet)) {
					[str appendFormat:@" ltw_valid_flag: %d\n", get_af_ltw_valid_flag(&packet)];					
					[str appendFormat:@" ltw_offset: %d\n", get_af_ltw_offset(&packet)];
				}
				if (get_af_piecewise_rate_flag(&packet)) {
					[str appendFormat:@" piecewise_rate: %d\n", get_af_piecewise_rate(&packet)];
				}
				if (get_af_seamless_splice_flag(&packet)) {
					[str appendFormat:@" splice_type: %d\n", get_af_splice_type(&packet)];
					[str appendFormat:@" DTS_next_AU: %d\n", get_af_DTS_next_AU(&packet)];
				}
			}
		}
	}
	
	[text_view_ setString:str];
}

@end
