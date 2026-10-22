apply(from = "shared/gradle/android-settings.gradle")

rootProject.name = "Kairo68"
include(":kairo68", ":frontend")
project(":frontend").projectDir = file("shared/frontend")
