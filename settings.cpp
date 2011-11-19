/***************************************************************************
    This file is part of Project Lemon
    Copyright (C) 2011 Zhipeng Jia

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#include "settings.h"
#include "compiler.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
}

int Settings::getDefaultFullScore() const
{
    return defaultFullScore;
}

int Settings::getDefaultTimeLimit() const
{
    return defaultTimeLimit;
}

int Settings::getDefaultMemoryLimit() const
{
    return defaultMemoryLimit;
}

int Settings::getCompileTimeLimit() const
{
    return compileTimeLimit;
}

int Settings::getSpecialJudgeTimeLimit() const
{
    return specialJudgeTimeLimit;
}

int Settings::getFileSizeLimit() const
{
    return fileSizeLimit;
}

int Settings::getNumberOfThreads() const
{
    return numberOfThreads;
}

const QString& Settings::getDefaultInputFileExtension() const
{
    return defaultInputFileExtension;
}

const QString& Settings::getDefaultOutputFileExtension() const
{
    return defaultOutputFileExtension;
}

const QStringList& Settings::getInputFileExtensions() const
{
    return inputFileExtensions;
}

const QStringList& Settings::getOutputFileExtensions() const
{
    return outputFileExtensions;
}

const QStringList& Settings::getRecentContest() const
{
    return recentContest;
}

const QList<Compiler*>& Settings::getCompilerList() const
{
    return compilerList;
}

const QString& Settings::getUiLanguage() const
{
    return uiLanguage;
}

void Settings::setDefaultFullScore(int score)
{
    defaultFullScore = score;
}

void Settings::setDefaultTimeLimit(int limit)
{
    defaultTimeLimit = limit;
}

void Settings::setDefaultMemoryLimit(int limit)
{
    defaultMemoryLimit = limit;
}

void Settings::setCompileTimeLimit(int limit)
{
    compileTimeLimit = limit;
}

void Settings::setSpecialJudgeTimeLimit(int limit)
{
    specialJudgeTimeLimit = limit;
}

void Settings::setFileSizeLimit(int limit)
{
    fileSizeLimit = limit;
}

void Settings::setNumberOfThreads(int number)
{
    numberOfThreads = number;
}

void Settings::setDefaultInputFileExtension(const QString &extension)
{
    defaultInputFileExtension = extension;
}

void Settings::setDefaultOutputFileExtension(const QString &extension)
{
    defaultOutputFileExtension = extension;
}

void Settings::setInputFileExtensions(const QString &extensions)
{
    inputFileExtensions = extensions.split(";", QString::SkipEmptyParts);
}

void Settings::setOutputFileExtensions(const QString &extensions)
{
    outputFileExtensions = extensions.split(";", QString::SkipEmptyParts);
}

void Settings::setRecentContest(const QStringList &list)
{
    recentContest = list;
}

void Settings::setUiLanguage(const QString &language)
{
    uiLanguage = language;
}

void Settings::addCompiler(Compiler *compiler)
{
    compiler->setParent(this);
    compilerList.append(compiler);
}

void Settings::deleteCompiler(int index)
{
    if (0 <= index && index < compilerList.size()) {
        delete compilerList[index];
        compilerList.removeAt(index);
    }
}

Compiler* Settings::getCompiler(int index)
{
    if (0 <= index && index < compilerList.size())
        return compilerList[index];
    else
        return 0;
}

void Settings::swapCompiler(int a, int b)
{
    if (0 <= a && a < compilerList.size())
        if (0 <= b && b < compilerList.size())
            compilerList.swap(a, b);
}

void Settings::copyFrom(Settings *other)
{
    setDefaultFullScore(other->getDefaultFullScore());
    setDefaultTimeLimit(other->getDefaultTimeLimit());
    setDefaultMemoryLimit(other->getDefaultMemoryLimit());
    setCompileTimeLimit(other->getCompileTimeLimit());
    setSpecialJudgeTimeLimit(other->getSpecialJudgeTimeLimit());
    setFileSizeLimit(other->getFileSizeLimit());
    setNumberOfThreads(other->getNumberOfThreads());
    setDefaultInputFileExtension(other->getDefaultInputFileExtension());
    setDefaultOutputFileExtension(other->getDefaultOutputFileExtension());
    setInputFileExtensions(other->getInputFileExtensions().join(";"));
    setOutputFileExtensions(other->getOutputFileExtensions().join(";"));
    
    for (int i = 0; i < compilerList.size(); i ++)
        delete compilerList[i];
    compilerList.clear();
    const QList<Compiler*> &list = other->getCompilerList();
    for (int i = 0; i < list.size(); i ++) {
        Compiler *compiler = new Compiler;
        compiler->copyFrom(list[i]);
        addCompiler(compiler);
    }
}

void Settings::saveSettings()
{
    QSettings settings("Crash", "Lemon");
    
    settings.setValue("UiLanguage", uiLanguage);
    
    settings.beginGroup("GeneralSettings");
    settings.setValue("DefaultFullScore", defaultFullScore);
    settings.setValue("DefaultTimeLimit", defaultTimeLimit);
    settings.setValue("DefaultMemoryLimit", defaultMemoryLimit);
    settings.setValue("CompileTimeLimit", compileTimeLimit);
    settings.setValue("SpecialJudgeTimeLimit", specialJudgeTimeLimit);
    settings.setValue("FileSizeLimit", fileSizeLimit);
    settings.setValue("NumberOfThreads", numberOfThreads);
    settings.setValue("DefaultInputFileExtension", defaultInputFileExtension);
    settings.setValue("DefaultOutputFileExtension", defaultOutputFileExtension);
    settings.setValue("InputFileExtensions", inputFileExtensions);
    settings.setValue("OutputFileExtensions", outputFileExtensions);
    settings.endGroup();
    
    settings.beginWriteArray("CompilerSettings");
    for (int i = 0; i < compilerList.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("CompilerName", compilerList[i]->getCompilerName());
        settings.setValue("SourceExtensions", compilerList[i]->getSourceExtensions());
        settings.setValue("Location", compilerList[i]->getLocation());
        QStringList configurationNames = compilerList[i]->getConfigurationNames();
        QStringList configurationSettings = compilerList[i]->getConfigurationSettings();
        settings.beginWriteArray("Configuration");
        for (int j = 0; j < configurationNames.size(); j ++) {
            settings.setArrayIndex(j);
            settings.setValue("Name", configurationNames[j]);
            settings.setValue("Arguments", configurationSettings[j]);
        }
        settings.endArray();
    }
    settings.endArray();
    
    settings.beginWriteArray("RecentContest");
    for (int i = 0; i < recentContest.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("Location", recentContest[i]);
    }
    settings.endArray();
}

void Settings::loadSettings()
{
    for (int i = 0; i < compilerList.size(); i ++)
        delete compilerList[i];
    compilerList.clear();
    recentContest.clear();
    
    QSettings settings("Crash", "Lemon");
    
    uiLanguage = settings.value("UiLanguage", QLocale::system().name()).toString();
    
    settings.beginGroup("GeneralSettings");
    defaultFullScore = settings.value("DefaultFullScore", 10).toInt();
    defaultTimeLimit = settings.value("DefaultTimeLimit", 1000).toInt();
    defaultMemoryLimit = settings.value("DefaultMemoryLimit", 64).toInt();
    compileTimeLimit = settings.value("CompileTimeLimit", 10000).toInt();
    specialJudgeTimeLimit = settings.value("SpecialJudgeTimeLimit", 10000).toInt();
    fileSizeLimit = settings.value("FileSizeLimit", 50).toInt();
    numberOfThreads = settings.value("NumberOfThreads", 1).toInt();
    defaultInputFileExtension = settings.value("DefaultInputFileExtension", "in").toString();
    defaultOutputFileExtension = settings.value("DefaultOuputFileExtension", "out").toString();
    inputFileExtensions = settings.value("InputFileExtensions", QStringList() << "in").toStringList();
    outputFileExtensions = settings.value("OutputFileExtensions", QStringList() << "out" << "ans").toStringList();
    settings.endGroup();
    
    int compilerCount = settings.beginReadArray("CompilerSettings");
    for (int i = 0; i < compilerCount; i ++) {
        settings.setArrayIndex(i);
        Compiler *compiler = new Compiler;
        compiler->setCompilerName(settings.value("CompilerName").toString());
        compiler->setSourceExtensions(settings.value("SourceExtensions").toStringList().join(";"));
        compiler->setLocation(settings.value("Location").toString());
        int configurationCount = settings.beginReadArray("Configuration");
        for (int j = 0; j < configurationCount; j ++) {
            settings.setArrayIndex(j);
            compiler->addConfiguration(settings.value("Name").toString(),
                                       settings.value("Arguments").toString());
        }
        settings.endArray();
        addCompiler(compiler);
    }
    settings.endArray();
    
    int listCount = settings.beginReadArray("RecentContest");
    for (int i = 0; i < listCount; i ++) {
        settings.setArrayIndex(i);
        recentContest.append(settings.value("Location").toString());
    }
    settings.endArray();
}

int Settings::upperBoundForFullScore()
{
    return 100;
}

int Settings::upperBoundForTimeLimit()
{
    return 1000 * 60 * 10;
}

int Settings::upperBoundForMemoryLimit()
{
    return 1024;
}

int Settings::upperBoundForFileSizeLimit()
{
    return 10 * 1024;
}

int Settings::upperBoundForNumberOfThreads()
{
    return 8;
}

QString Settings::dataPath()
{
    return QString("data") + QDir::separator();
}

QString Settings::sourcePath()
{
    return QString("source") + QDir::separator();
}

QString Settings::temporaryPath()
{
    return QString("temp") + QDir::separator();
}

QString Settings::selfTestPath()
{
    return QString("selftest") + QDir::separator();
}