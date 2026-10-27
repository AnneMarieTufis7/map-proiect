// Starter skeleton for the MAP project.
// Pure logic lives here so it can be unit tested without starting a server.
#pragma once


#include <algorithm>
#include <numeric>
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <string>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>

namespace map_project {

inline constexpr const char* kAppName = "map-project";
inline constexpr const char* kAppVersion = "0.1.0";

/// Reads an environment variable, falling back when it is missing or empty.
inline std::string Env(const char* key, const std::string& fallback) {
    const char* value = std::getenv(key);
    return (value == nullptr || *value == '\0') ? fallback : std::string(value);
}

// Structurile de date pentru Tema 3: Catalog de note
struct Student {
    int id;
    std::string name;
    std::string group;
};

// Macro nlohmann/json pentru a serializa automat structura Student
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Student, id, name, group)

struct Grade {
    int student_id;
    std::string subject;
    int grade;
};

/// Holds the application data in memory.
class Store {
public:
    void Reset() {
        std::lock_guard<std::mutex> guard(mutex_);
        // Numerotarea reîncepe obligatoriu de la 1 la apelarea rutei /reset
        next_student_id_ = 1;
        // Golește toate datele din memorie[cite: 32]
        students_.clear();
        grades_.clear();
    }

    int TakeNextStudentId() {
        std::lock_guard<std::mutex> guard(mutex_);
        return next_student_id_++;
    }

    // Funcții ajutătoare pentru manipularea datelor în memorie
    void AddStudent(const Student& student) {
        std::lock_guard<std::mutex> guard(mutex_);
        students_[student.id] = student;
    }

    bool StudentExists(int id) {
        std::lock_guard<std::mutex> guard(mutex_);
        return students_.find(id) != students_.end();
    }

    void AddGrade(const Grade& grade) {
        std::lock_guard<std::mutex> guard(mutex_);
        grades_.push_back(grade);
    }

    // Funcție pentru a verifica regula: max K=5 note la aceeași materie
    int CountGradesForSubject(int student_id, const std::string& subject) {
        std::lock_guard<std::mutex> guard(mutex_);
        int count = 0;
        for (const auto& g : grades_) {
            if (g.student_id == student_id && g.subject == subject) {
                count++;
            }
        }
        return count;
    }

    // Getter pentru teste / rute
    std::map<int, Student> GetAllStudents() {
        std::lock_guard<std::mutex> guard(mutex_);
        return students_;
    }

    std::vector<Grade> GetGradesForStudent(int student_id) {
        std::lock_guard<std::mutex> guard(mutex_);
        std::vector<Grade> student_grades;
        for (const auto& g : grades_) {
            if (g.student_id == student_id) {
                student_grades.push_back(g);
            }
        }
        return student_grades;
    }

private:
    std::mutex mutex_;
    int next_student_id_ = 1;
    
    // Containerele propriu-zise pentru stocarea datelor
    std::map<int, Student> students_;
    std::vector<Grade> grades_;
};

/// Seconds since the process started.
inline long UptimeSeconds(std::chrono::steady_clock::time_point started_at) {
    const auto elapsed = std::chrono::steady_clock::now() - started_at;
    return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
}

inline nlohmann::json HealthBody(std::chrono::steady_clock::time_point started_at) {
    return {{"status", "ok"}, {"uptime_seconds", UptimeSeconds(started_at)}};
}

inline nlohmann::json VersionBody() {
    return {
        {"app", kAppName},
        {"version", kAppVersion},
        {"commit", Env("APP_COMMIT", "dev")},
        {"built_at", Env("APP_BUILT_AT", "unknown")},
    };
}

inline nlohmann::json ErrorBody(const std::string& code, const std::string& message) {
    return {{"error", code}, {"message", message}};
}

inline std::string HomePage() {
    return std::string(R"(<!DOCTYPE html>
<html lang="ro"><head><meta charset="utf-8"><title>)") + kAppName + R"(</title></head>
<body>
<h1>)" + kAppName + R"(</h1>
<p>Autor: Tufiș Anne-Marie, grupa 2.2</p> 
<p>Tema: 3 (Catalog de note)</p>
<p>Versiune: )" + kAppVersion + ", commit " + Env("APP_COMMIT", "dev") + R"(</p>
</body></html>)";
}

}  // namespace map_project

