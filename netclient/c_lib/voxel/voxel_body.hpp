#pragma once

/* Dat storage */

class VoxColors {
    public:
        unsigned char** rgba;
        int **index;
        int n;

        bool team;
        unsigned char team_r,team_g,team_b;
        
        void init(int dx, int dy, int dz);
        void set(int i, int x, int y, int z, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void set_team_base(unsigned char r, unsigned char g, unsigned char b);
        void set_team(bool team);
        VoxColors();
        ~VoxColors();
};

//class VoxPartRotation {
    //public:
        //float fx,fy,fz;     // internal orientation
        //float nx,ny,nz; // orientation vector relative to main anchor point ??

        //void set(float fx, float fy, float fz, float nx, float ny, float nz);

        //VoxPartRotation();
        //VoxPartRotation(float fx, float fy, float fz, float nx, float ny, float nz);
//};

class VoxPartAnchor {
    public:
        float length;
        float x,y,z;

        void set(float length, float x, float y, float z);

        VoxPartAnchor();
        VoxPartAnchor(float length, float x, float y, float z);
};

class VoxPartDimension {
    public:
        int x,y,z;

        void set(int x, int y, int z);
        int count();

        VoxPartDimension();
        VoxPartDimension(int x, int y, int z);
};

struct VoxPart {
    public:
        //VoxPartRotation rotation;
        VoxPartAnchor anchor;
        VoxPartDimension dimension;
        VoxColors colors;
        
        int part_num;
        float vox_size;
        bool biaxial; // true for horizontal+vertical (head). default=false

        void set_rotation(float fx, float fy, float fz, float nx, float ny, float nz);
        void set_anchor(float len, float x, float y, float z);
        void set_dimension(int x, int y, int z);

        VoxPart(
            float rot_fx, float rot_fy, float rot_fz,
            float rot_nx, float rot_ny, float rot_nz,
            float anc_len, float anc_x, float anc_y, float anc_z,
            int dim_x, int dim_y, int dim_z,
            int part_num,
            bool biaxial=false
        );
};

class VoxBody {
    public:
        class VoxPart** vox_part;
        bool inited;
        int n_parts;

        void init(int n_parts);

        void set_part(
            int part_num,
            float vox_size,
            float rot_fx, float rot_fy, float rot_fz,
            float rot_nx, float rot_ny, float rot_nz,
            float anc_len, float anc_x, float anc_y, float anc_z,
            int dim_x, int dim_y, int dim_z,
            bool biaxial=false
        );
        void set_color(int part, int x, int y, int z, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void set_team(int part, bool team, unsigned char r, unsigned char g, unsigned char b);

        void where() {
            printf("VoxBody instantiated at: %p\n", this);
        }

        VoxBody();
        ~VoxBody();
};
