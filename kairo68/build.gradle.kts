plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

extra["kairoProduct"] = "kairo68"
extra["kairoLabel"] = "Kairo68"
apply(from = rootProject.file("shared/gradle/kairo-machine-app.gradle"))
