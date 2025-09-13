//
//  usgmacApp.swift
//  usgmac
//
//  Created by Emile Antat on 13/09/2025.
//

import AppKit
import SwiftUI
import UsgmacKit

@main
struct usgmacApp: App {
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    var body: some Scene {
        Settings {
            EmptyView()
        }
    }
}

class AppDelegate: NSObject, NSApplicationDelegate {
    private var statusItem: NSStatusItem?
    private var timer: Timer?

    func applicationDidFinishLaunching(_ notification: Notification) {
        NSApp.setActivationPolicy(.accessory)
        statusItem = NSStatusBar.system.statusItem(
            withLength: NSStatusItem.variableLength
        )

        // Allow user to quit the application.
        let menu = NSMenu()
        let quitMenuItem = NSMenuItem(
            title: "Quit",
            action: #selector(NSApplication.terminate(_:)),
            keyEquivalent: "q"
        )
        menu.addItem(quitMenuItem)
        statusItem?.menu = menu

        // Timer to refresh CPU Usage.
        timer = Timer.scheduledTimer(withTimeInterval: 3.5, repeats: true) {
            _ in
            self.statusItem?.button?.title =
                "CPU: \(UsgmacKit.um_cpu_notify().value)%"
        }
    }
}
