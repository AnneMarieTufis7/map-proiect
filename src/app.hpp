// Starter skeleton for the MAP project.
// Pure logic lives here so it can be unit tested without starting a server.
#pragma once

#include <chrono>
#include <cstdlib>
#include <mutex>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <optional>

#include <nlohmann/json.hpp>

namespace map_project {

inline constexpr const char* kAppName = "map-project";
inline constexpr const char* kAppVersion = "0.1.0";

/// Reads an environment variable, falling back when it is missing or empty.
inline std::string Env(const char* key, const std::string& fallback) {
    const char* value = std::getenv(key);
    return (value == nullptr || *value == '\0') ? fallback : std::string(value);
}

struct Student {
    int id;
    std::string name;
    std::string group;
};

// Serializare automată JSON pentru structura Student
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Student, id, name, group)

struct Grade {
    int student_id;
    std::string subject;
    int grade;
};

// Serializare automată JSON pentru structura Grade
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Grade, student_id, subject, grade)

// Funcție globală matematică pentru rotunjire la 2 zecimale (mutată deasupra clasei Store)
inline double RoundToTwoDecimals(double value) {
    return std::round(value * 100.0) / 100.0;
}

class Store {
public:
    void Reset() {
        std::lock_guard<std::mutex> guard(mutex_);
        next_id_ = 1;
        students_.clear();
        grades_.clear();
    }

    int NextId() {
        std::lock_guard<std::mutex> guard(mutex_);
        return next_id_;
    }

    int TakeNextId() {
        std::lock_guard<std::mutex> guard(mutex_);
        return next_id_++;
    }

    void AddStudent(const Student& student) {
        std::lock_guard<std::mutex> guard(mutex_);
        students_[student.id] = student;
    }

    bool StudentExists(int id) {
        std::lock_guard<std::mutex> guard(mutex_);
        return students_.find(id) != students_.end();
    }

    std::vector<Student> GetAllStudentsOrderedByName() {
        std::lock_guard<std::mutex> guard(mutex_);
        std::vector<Student> result;
        for (const auto& pair : students_) {
            result.push_back(pair.second);
        }
        std::sort(result.begin(), result.end(), [](const Student& a, const Student& b) {
            if (a.name == b.name) return a.id < b.id;
            return a.name < b.name;
        });
        return result;
    }

    void AddGrade(const Grade& grade) {
        std::lock_guard<std::mutex> guard(mutex_);
        grades_.push_back(grade);
    }

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

    std::optional<double> CalculateStudentAverage(int student_id) {
        std::lock_guard<std::mutex> guard(mutex_);
        double sum = 0;
        int count = 0;
        for (const auto& g : grades_) {
            if (g.student_id == student_id) {
                sum += g.grade;
                count++;
            }
        }
        if (count == 0) {
            return std::nullopt;
        }
        return RoundToTwoDecimals(sum / count);
    }

    std::vector<Grade> GetStudentGrades(int student_id) {
        std::lock_guard<std::mutex> guard(mutex_);
        std::vector<Grade> result;
        for (const auto& g : grades_) {
            if (g.student_id == student_id) {
                result.push_back(g);
            }
        }
        return result;
    }

    std::vector<int> GetGradesForReport(const std::string& subject) {
        std::lock_guard<std::mutex> guard(mutex_);
        std::vector<int> result;
        for (const auto& g : grades_) {
            if (g.subject == subject) {
                result.push_back(g.grade);
            }
        }
        return result;
    }

private:
    std::mutex mutex_;
    int next_id_ = 1;
    std::map<int, Student> students_;
    std::vector<Grade> grades_;
};

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
<p>Tema: 3</p>
<p>Versiune: )" + kAppVersion + ", commit " + Env("APP_COMMIT", "dev") + R"(</p>
</body></html>)";
}

}  // namespace map_project