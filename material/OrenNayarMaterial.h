//
// Created by Filip K on 05/12/16.
//

#ifndef TNCG15_PROJ_ORENNAYARMATERIAL_H
#define TNCG15_PROJ_ORENNAYARMATERIAL_H

class OrenNayarMaterial : public BaseMaterial {
public:
    OrenNayarMaterial(ColorRGB _color) : BaseMaterial(_color) { };

    ColorRGB BRDF(vec3 incoming_direction, vec3 outgoing_direction, vec3 collision_normal) {
        float albedo = 1.8f;
        float roughness = 1.0f;
        vec3 view_direction = -1.0f * incoming_direction;

        float NdotL = dot(collision_normal, outgoing_direction);
        float NdotV = dot(collision_normal, view_direction);

        float angleVN = acos(NdotV);
        float angleLN = acos(NdotL);

        float alpha = std::max(angleVN, angleLN);
        float beta = min(angleVN, angleLN);
        float gamma = dot(view_direction - collision_normal * dot(view_direction, collision_normal), outgoing_direction - collision_normal * dot(outgoing_direction, collision_normal));

        float roughnessSquared = roughness * roughness;

        float A = 1.0f - 0.5f * (roughnessSquared / (roughnessSquared + 0.57f));
        float B = 0.45f * (roughnessSquared / (roughnessSquared + 0.09f));
        float C = sin(alpha) * tan(beta);

        float L1 = std::max(0.0f, NdotL) * (A + B * std::max(0.0f, gamma) * C);

        return get_color() * L1;
    }

    void PDF(vec3 ray_direction, vec3 collision_normal, vec3 &reflected_dir, vec3 &transmitted_dir, float &radiance_dist) {
        if ((((float) rand()/RAND_MAX) < 0.8)) {
            float r1 = 2 * 3.14159f * ((float) rand() / RAND_MAX);// pick random number on unit circle (radius = 1, circumference = 2*Pi) for azimuth
            float r2 = ((float) rand() / RAND_MAX);  // pick random number for elevation
            float r2s = sqrtf(r2);

            vec3 w = collision_normal;
            vec3 u = normalize(cross((fabs(w.x) > .1 ? vec3(0, 1, 0) : vec3(1, 0, 0)), w));
            vec3 v = cross(w,u);

            reflected_dir = normalize(u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrtf(1 - r2));
            radiance_dist = 1.0f;
        }
    }


    int get_material_type() { return BaseMaterial::OREN_NAYAR; }
};

#endif //TNCG15_PROJ_ORENNAYARMATERIAL_H
