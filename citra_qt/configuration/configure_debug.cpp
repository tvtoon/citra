// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <QDesktopServices>
#include <QUrl>
#include "citra_qt/configuration/configure_debug.h"
#include "citra_qt/debugger/console.h"
#include "citra_qt/uisettings.h"
#include "common/file_util.h"
#include "common/logging/backend.h"
#include "common/logging/filter.h"
#include "common/logging/log.h"
#include "core/core.h"
#include "core/settings.h"
#include "ui_configure_debug.h"

ConfigureDebug::ConfigureDebug(QWidget* parent)
    : QWidget(parent), ui(std::make_unique<Ui::ConfigureDebug>()) {
    ui->setupUi(this);
    SetConfiguration();

    connect(ui->open_log_button, &QPushButton::clicked, []() {
        QString path = QString::fromStdString(FileUtil::GetUserPath(FileUtil::UserPath::LogDir));
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    });
    ui->toggle_cpu_jit->setEnabled(!Core::System::GetInstance().IsPoweredOn());
}

ConfigureDebug::~ConfigureDebug() = default;

void ConfigureDebug::SetConfiguration() {
    ui->toggle_gdbstub->setChecked(Settings::values.use_gdbstub);
    ui->gdbport_spinbox->setEnabled(Settings::values.use_gdbstub);
    ui->gdbport_spinbox->setValue(Settings::values.gdbstub_port);
    ui->toggle_console->setEnabled(!Core::System::GetInstance().IsPoweredOn());
    ui->toggle_console->setChecked(UISettings::values.show_console);
    ui->log_filter_edit->setText(QString::fromStdString(Settings::values.log_filter));
    ui->toggle_cpu_jit->setChecked(Settings::values.use_cpu_jit);
}

void ConfigureDebug::ApplyConfiguration() {
    Settings::values.use_gdbstub = ui->toggle_gdbstub->isChecked();
    Settings::values.gdbstub_port = ui->gdbport_spinbox->value();
    UISettings::values.show_console = ui->toggle_console->isChecked();
    Settings::values.log_filter = ui->log_filter_edit->text().toStdString();
    Debugger::ToggleConsole();
    Log::Filter filter;
    filter.ParseFilterString(Settings::values.log_filter);
    Log::SetGlobalFilter(filter);
    Settings::values.use_cpu_jit = ui->toggle_cpu_jit->isChecked();
}

void ConfigureDebug::RetranslateUI() {
    ui->retranslateUi(this);
}
