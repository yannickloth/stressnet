/* cmdline.h */

/* File autogenerated by gengetopt version 2.10  */

#ifndef CMDLINE_H
#define CMDLINE_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef CMDLINE_PARSER_PACKAGE
#define CMDLINE_PARSER_PACKAGE "stressnet"
#endif

#ifndef CMDLINE_PARSER_VERSION
#define CMDLINE_PARSER_VERSION "0.1"
#endif

struct gengetopt_args_info
{
  char * interface_arg;	/* network device used to send packets (default='eth0').  */
  char * packetfileM_arg;	/* First file of packets.  */
  char * packetfileN_arg;	/* Second file ofpackets.  */
  long ratio_arg;	/* ratio M/N: number of packets of file M for every 1 packet of file N (default='50').  */
  long bitrate_arg;	/* desired send bitrate in kbit/s (default='100').  */
  char * configfile_arg;	/* config file containing the command line arguments.  */
  int priority_arg;	/* scheduling priority value (default='0').  */
  int quantity_arg;	/* total quantity of packets to send (default='100').  */
  char * destMAC_arg;	/* destination MAC address for ALL packets (default='00:11:22:33:44:55').  */

  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int interface_given ;	/* Whether interface was given.  */
  int packetfileM_given ;	/* Whether packetfileM was given.  */
  int packetfileN_given ;	/* Whether packetfileN was given.  */
  int ratio_given ;	/* Whether ratio was given.  */
  int bitrate_given ;	/* Whether bitrate was given.  */
  int configfile_given ;	/* Whether configfile was given.  */
  int priority_given ;	/* Whether priority was given.  */
  int quantity_given ;	/* Whether quantity was given.  */
  int destMAC_given ;	/* Whether destMAC was given.  */
  int stats_given ;	/* Whether stats was given.  */

} ;

int cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info);

void cmdline_parser_print_help(void);
void cmdline_parser_print_version(void);

int cmdline_parser_configfile (char * const filename, struct gengetopt_args_info *args_info, int override);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* CMDLINE_H */