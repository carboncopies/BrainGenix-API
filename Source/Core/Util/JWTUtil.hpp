#pragma once

#include <string>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/traits.h> // Using kazuho-picojson consistently

class JWTUtil {
private:
    inline static const std::string SECRET_KEY = "MyVerySecureToken";
    inline static const std::string ISSUER = "BrainGenix";

public:
    static std::string generateToken(const std::string& username, const std::string& role = "admin") {
        return jwt::create()
            .set_issuer(ISSUER)
            .set_subject(username)
            .set_payload_claim("role", jwt::claim(role))
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(12))
            .sign(jwt::algorithm::hs256{SECRET_KEY});
    }

    static jwt::decoded_jwt<jwt::traits::kazuho_picojson> verifyToken(const std::string& token) {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
            .with_issuer(ISSUER);
        verifier.verify(decoded);
        return decoded;
    }

    static std::string getRole(const jwt::decoded_jwt<jwt::traits::kazuho_picojson>& decoded) {
        return decoded.get_payload_claim("role").as_string();
    }

    static std::string getUsername(const jwt::decoded_jwt<jwt::traits::kazuho_picojson>& decoded) {
        return decoded.get_subject();
    }
};