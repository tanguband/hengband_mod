/**
 * \file AppDelegate.m
 * \brief This is a minimal implementation of the OS X front end.
 *
 * Use this file to rebuild the .nib file with Xcode without having to pull
 * in all of the Hengband source.  This is the procedure with Xcode 12:
 *
 * 1) Create a new Xcode project for a macOS App.
 * 2) You can set the "Product Name", "Team", "Organization Name",
 *    "Organization Identifier" as you wish.  Setting the product name to
 *    "hengband" and the "Organization Identifier" to "jp.osdn" will match
 *    the bundle identifier used in the full builds for Hengband.  Set
 *    "Language" to "Objective-C" and "User Interface" to "XIB".  Leave
 *    "Use Core Data" off.  The setting for "Include Tests" doesn't matter;
 *    you can turn it off to avoid extra clutter.
 * 3) In hengband's project settings on the "Info" tab, set the deployment
 *    target to what's used in Hengband's src/Makefile.am.  When this was
 *    written, that was 10.8 and 10.8 is necessary for Base localization.
 *    In the localizations part of that tab, click the '+' and add a Japanese
 *    localization.  That will prompt you for the files involved.  Leave that
 *    as is:  one file, "MainMenu.xib", with Base as the reference language
 *    and localizable strings as the file type.
 * 4) In hengband's targets on the "General" tab, verify that "Main Interface"
 *    is MainMenu.
 * 5) Copy src/cocoa/AppDelegate.h and src/cocoa/AppDelegate.m from the
 *    Hengband source files to the directory in the project with main.m.  Copy
 *    src/cocoa/Base.lproj/MainMenu.xib to the Base.lproj subdirectory of that
 *    directory.  Copy src/cocoa/ja.lproj/MainMenu.strings to the ja.lproj
 *    subdirectory of that directory.
 * 6) (This annoyance seems to have gone away beween Xcode 11 and Xcode 13;
 *    leaving it here just in case) If you modify MainMenu.xib after copying
 *    it over, you may want to set it so that it can be opened in older
 *    versions of Xcode.  Select it in Xcode, and select one of the things,
 *    like "File's Owner" from it.  In the file information panel for it,
 *    there will be a section labeled "Document Editing" with an option
 *    menu for "Opens in".  Choosing one of the options other than
 *    "Latest Xcode" will close the file and save it with the appropriate
 *    flags.  Note that reopening the xib file in Xcode and saving it will
 *    cause the version to revert to the latest Xcode.
 * 7) If you want to change the Japanese strings for the menus, one way to
 *    partly do it in Xcode is to export the localizations:  from the file view
 *    select topmost category ("hengband" with an application icon) and then
 *    select Editor->Export for Localization... in Xcode's menu bar.  That
 *    will prompt you for where to save the exported localizations.  That
 *    export is done as a directory tree.  Within it, you'll find a
 *    ja.xcloc/Localized Contents/ja.xliff file.  The strings bracketed with
 *    <source></source> in that file are the English strings.  The strings
 *    bracketed with <target></target> give what's currently used for the
 *    Japanese version.  Adjust the strings bracketed with <target></target>,
 *    save the modified file, and use Editor->Import Localizations... from
 *    within Xcode to import the localization from the ja.xloc directory.
 *    The result of that will be to regenerate ja.lproj/MainMenu.strings in the
 *    Xcode project files which you can use to replace the version in
 *    src/cocoa/ja.lproj/MainMenu.strings in the Hengband source code.
 * 8) Use Xcode's Product->Build For->Running menu entry to build the project.
 * 9) The generated .nib file for English will be
 *    Contents/Resources/Base.lproj/MainMenu.nib in the product directory which
 *    is something like
 *    ~/Library/Developer/Xcode/DerivedData/<product_name>-<some_string>/Build/Products/Debug/<product_name>.app
 *    You can use it to replace the src/cocoa/Base.lproj/MainMenu.nib in the
 *    Hengband source files.  With Xcode 13, the generated .nib files are
 *    directories.  In the build results from that version of Xcode,
 *    copy Contents/Resources/Base.lproj/MainMenu.nib/keyedobjects.nib to
 *    replace src/cocoa/Base.lproj/MainMenu.nib in the Hengband source files
 *    (the keyedobjects-101300.nib file in the build results is for
 *    macOS 10.13 or later).
 *
 * This work is free software; you can redistribute it and/or modify it
 * under the terms of either:
 *
 * a) the GNU General Public License as published by the Free Software
 *    Foundation, version 2, or
 *
 * b) the "Angband licence":
 *    This software may be copied and distributed for educational, research,
 *    and not for profit purposes provided that this copyright and statement
 *    are included in all such copies.  Other copyrights may also apply.
 */

#import "AppDelegate.h"

@implementation AngbandAppDelegate

@synthesize graphicsMenu=_graphicsMenu;
@synthesize commandMenu=_commandMenu;
@synthesize commandMenuTagMap=_comandMenuTagMap;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (IBAction)newGame:(id)sender {
}

- (IBAction)editFont:(id)sender {
}

- (IBAction)openGame:(id)sender {
}

- (IBAction)saveGame:(id)sender {
}

- (IBAction)setRefreshRate:(NSMenuItem *)sender {
}

- (IBAction)toggleSound:(NSMenuItem*)menuItem {
}

- (IBAction)toggleWideTiles:(NSMenuItem *)sender {
}

- (IBAction)setGraphicsMode:(NSMenuItem *)sender {
}

- (void)selectWindow:(id)sender {
}

- (void)beginGame {
}

@end
