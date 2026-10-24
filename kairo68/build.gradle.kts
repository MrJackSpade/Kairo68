plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

extra["kairoProduct"] = "kairo68"
extra["kairoLabel"] = "Kairo68"
extra["kairoCatalogRules"] = "com.mrjackspade.kairo68.X68kCatalog"
apply(from = rootProject.file("shared/gradle/kairo-app.gradle"))
