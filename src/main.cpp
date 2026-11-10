// Wires the HTTP routes of the common contract.
// Add the routes required by your assigned theme below.

#include <chrono>
#include <iostream>

#include <httplib.h>

#include "app.hpp"

int main() {
    using namespace map_project;

    const auto started_at = std::chrono::steady_clock::now();
    Store store;
    httplib::Server server;

    // Rutele comune existente din șablon
    server.Get("/health", [&](const httplib::Request&, httplib::Response& response) {
        response.set_content(HealthBody(started_at).dump(), "application/json; charset=utf-8");
    });

    server.Get("/version", [](const httplib::Request&, httplib::Response& response) {
        response.set_content(VersionBody().dump(), "application/json; charset=utf-8");
    });

    server.Post("/reset", [&](const httplib::Request&, httplib::Response& response) {
        store.Reset();
        response.status = 204;
    });

    server.Get("/", [](const httplib::Request&, httplib::Response& response) {
        response.set_content(HomePage(), "text/html; charset=utf-8");
    });

    // --- RUTELE SPECIFICE TEMEI 3 (Catalog de note) ---

    // 1. POST /students - Adaugă un student nou
    server.Post("/students", [&](const httplib::Request& req, httplib::Response& response) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            Student student;
            student.id = store.TakeNextId();
            student.name = json_data.at("name").get<std::string>();
            student.group = json_data.at("group").get<std::string>();

            store.AddStudent(student);

            response.status = 201;
            response.set_content(nlohmann::json(student).dump(), "application/json; charset=utf-8");
        } catch (...) {
            response.status = 400;
            response.set_content(ErrorBody("bad_request", "invalid student data").dump(), "application/json; charset=utf-8");
        }
    });

    // 2. GET /students - Listează toți studenții alfabetic după nume
    server.Get("/students", [&](const httplib::Request&, httplib::Response& response) {
        auto students = store.GetAllStudentsOrderedByName();
        nlohmann::json json_result = students;
        response.set_content(json_result.dump(), "application/json; charset=utf-8");
    });

    // 3. POST /grades - Adaugă o notă pentru un student
    server.Post("/grades", [&](const httplib::Request& req, httplib::Response& response) {
        try {
            auto json_data = nlohmann::json::parse(req.body);
            Grade grade;
            grade.student_id = json_data.at("student_id").get<int>();
            grade.subject = json_data.at("subject").get<std::string>();
            grade.grade = json_data.at("grade").get<int>();

            // Verificăm dacă studentul există (altfel 404)
            if (!store.StudentExists(grade.student_id)) {
                response.status = 404;
                response.set_content(ErrorBody("not_found", "student does not exist").dump(), "application/json; charset=utf-8");
                return;
            }

            // Verificăm regula de maximum K=5 note la aceeași materie (altfel 409 Conflict)
            if (store.CountGradesForSubject(grade.student_id, grade.subject) >= 5) {
                response.status = 409;
                response.set_content(ErrorBody("conflict", "maximum 5 grades per subject allowed").dump(), "application/json; charset=utf-8");
                return;
            }

            store.AddGrade(grade);
            response.status = 201;
            response.set_content(nlohmann::json(grade).dump(), "application/json; charset=utf-8");
        } catch (...) {
            response.status = 400;
            response.set_content(ErrorBody("bad_request", "invalid grade data").dump(), "application/json; charset=utf-8");
        }
    });

    // 4. GET /report?subject=... - Raport pe disciplină
    server.Get("/report", [&](const httplib::Request& req, httplib::Response& response) {
        if (!req.has_param("subject")) {
            response.status = 400;
            response.set_content(ErrorBody("bad_request", "missing subject parameter").dump(), "application/json; charset=utf-8");
            return;
        }

        std::string subject = req.get_param_value("subject");
        auto grades = store.GetGradesForReport(subject);

        if (grades.empty()) {
            nlohmann::json report = {
                {"subject", subject},
                {"count", 0},
                {"average", nullptr},
                {"min", nullptr},
                {"max", nullptr}
            };
            response.set_content(report.dump(), "application/json; charset=utf-8");
            return;
        }

        int sum = 0;
        int min_val = grades[0];
        int max_val = grades[0];
        for (int g : grades) {
            sum += g;
            if (g < min_val) min_val = g;
            if (g > max_val) max_val = g;
        }

        double avg = RoundToTwoDecimals(static_cast<double>(sum) / grades.size());

        nlohmann::json report = {
            {"subject", subject},
            {"count", grades.size()},
            {"average", avg},
            {"min", min_val},
            {"max", max_val}
        };

        response.set_content(report.dump(), "application/json; charset=utf-8");
    });

    // Handler-ul de eroare 404
    server.set_error_handler([](const httplib::Request&, httplib::Response& response) {
        if (response.status == 404) {
            response.set_content(ErrorBody("not_found", "route does not exist").dump(),
                                   "application/json; charset=utf-8");
        }
    });

    std::cout << kAppName << " " << kAppVersion << " starting on port 8080" << std::endl;
    if (!server.listen("0.0.0.0", 8080)) {
        std::cerr << "failed to bind port 8080" << std::endl;
        return 1;
    }
    return 0;
}