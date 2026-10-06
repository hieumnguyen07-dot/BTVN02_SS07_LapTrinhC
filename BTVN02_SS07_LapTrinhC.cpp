#include <stdio.h>

struct SmartDevice {
    int deviceID;
    char deviceName[50];
    int status;
    float ratedPowerKW;
};

struct EnergySensor {
    float voltageVolts;
    float currentAmperes;
    int unoccupancyMinutes;
};

int main() {
    struct SmartDevice dev;
    struct EnergySensor sensor;
    float operatingHours;

    printf("HE THONG QUAN LY THIET BI THONG MINH (RIKKEI SMARTHOME)\n");
    printf("Nhap ma dinh danh thiet bi (ID): ");
    if (scanf("%d", &dev.deviceID) != 1) return 1;

    printf("Nhap ten thiet bi: ");
    if (scanf("%49s", dev.deviceName) != 1) return 1;

    printf("Nhap trang thai ban dau (1: Bat, 0: Tat): ");
    if (scanf("%d", &dev.status) != 1) return 1;

    printf("Nhap cong suat dinh muc (kW): ");
    if (scanf("%f", &dev.ratedPowerKW) != 1) return 1;

    printf("--- Nhap thong so tu cam bien ---\n");
    printf("Nhap dien ap (V): ");
    if (scanf("%f", &sensor.voltageVolts) != 1) return 1;

    printf("Nhap dong dien (A): ");
    if (scanf("%f", &sensor.currentAmperes) != 1) return 1;

    printf("Nhap thoi gian vang nguoi (phut): ");
    if (scanf("%d", &sensor.unoccupancyMinutes) != 1) return 1;

    printf("Nhap so gio van hanh trong thang (gio): ");
    if (scanf("%f", &operatingHours) != 1) return 1;

    if (sensor.voltageVolts < 0 || sensor.currentAmperes < 0 || sensor.unoccupancyMinutes < 0 || dev.ratedPowerKW < 0) {
        printf("\n[LOI DU LIEU 1] Phat hien thong so cam bi?n hoac cong suat bi am (< 0)! Loi phan cung.\n");
        printf("H? th?ng da huy thao tac xu ly.\n");
        return 0;
    }

    if (operatingHours <= 0 || operatingHours > 744) {
        printf("\n[LOI DU LIEU 2] So gio van hanh trong thang khong hop le (Phai tu > 0 den toi da 744 gio).\n");
        printf("H? th?ng tu choi tinh toan hoa don.\n");
        return 0;
    }

    int emergencyShutdown = 0;
    int energySavingShutdown = 0;

    if (sensor.currentAmperes > 30.0f) {
        dev.status = 0;
        emergencyShutdown = 1;
    } 
    else if (dev.status == 1 && sensor.unoccupancyMinutes >= 15) {
        dev.status = 0;
        energySavingShutdown = 1;
    }

    float totalKWh = dev.ratedPowerKW * operatingHours;
    double totalCost = 0;
    float remainingKWh = totalKWh;

    if (remainingKWh > 0) {
        float b1 = (remainingKWh > 50) ? 50 : remainingKWh;
        totalCost += b1 * 1806.0;
        remainingKWh -= b1;
    }
    if (remainingKWh > 0) {
        float b2 = (remainingKWh > 50) ? 50 : remainingKWh;
        totalCost += b2 * 1866.0;
        remainingKWh -= b2;
    }
    if (remainingKWh > 0) {
        float b3 = (remainingKWh > 100) ? 100 : remainingKWh;
        totalCost += b3 * 2167.0;
        remainingKWh -= b3;
    }
    if (remainingKWh > 0) {
        float b4 = remainingKWh;
        totalCost += b4 * 2729.0;
    }

    printf("------------------------------------------------------------\n");
    printf("                  BAO CAO VAN HANH THI?T B?                 \n");
    printf("------------------------------------------------------------\n");
    printf("Ma thiet bi    : %d\n", dev.deviceID);
    printf("Ten thiet bi   : %s\n", dev.deviceName);
    printf("Cong suat dinh muc: %.2f kW\n", dev.ratedPowerKW);
    printf("Trang thai cuoi cung: %s\n", (dev.status == 1) ? "Dang bat (1)" : "Da tat (0)");

    if (emergencyShutdown) {
        printf("[CANH BAO AN TOAN]: Phat hien dong dien qua tai (%.2f A > 30.0A). "
               "H? thong da ngat khan cap de phong chong chay no!\n", sensor.currentAmperes);
    } else if (energySavingShutdown) {
        printf("[THONG BAO TIET KIEM]: Phong vang nguoi lien tuc %d phut (>= 15 phut). "
               "H? thong da tu dong tat thiet bi!\n", sensor.unoccupancyMinutes);
    }

    printf("------------------------------------------------------------\n");
    printf("Tong dien nang tieu thu : %.2f kWh\n", totalKWh);
    printf("Tong tien dien (chua VAT): %.2f VNÐ\n", totalCost);
    printf("------------------------------------------------------------\n");

    return 0;
}
