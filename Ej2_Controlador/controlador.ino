float controlador1(float Rk, float Yk, float Tm)
{
  float Uk, Uks; //Señal de control y señal de control saturada
  float uk; //Señal de control incremental
  float Ueq = 0;
  static float Int_k = 0.0, Int_ks = 0.0;
  static float ek = 0.0, e_k_1 = 0.0;
  float Umax = 200, Umin = -200; /* Valores límite de la señal de control */
  float kp = -4.17540*5;
  float kd = -0.93737*5;

  /*Control*/
  ek = Rk - Yk; //Cálculo del error actual

  //Calculamos la señal de control incremental (PID)
  uk = kp * ek + kd * (ek - e_k_1) / Tm;

  e_k_1 = ek; //Actualizamos el error del instante anterior

  if (uk >= 5)
    Uk = Ueq + uk;
  else if (uk <= -5)
    Uk = -Ueq + uk;
  else
    Uk = 0;

  /* Saturación de la señal de control */
  if (Uk > Umax) Uks = Umax;
  else if (Uk < Umin) Uks = Umin;
  else Uks = Uk;

  return Uks;
 }
 float controlador2(float Rk, float Yk, float Tm)
{
  float Uk, Uks; //Señal de control y señal de control saturada
  float uk; //Señal de control incremental
  float Ueq = 0;
  static float Int_k = 0.0, Int_ks = 0.0;
  static float ek = 0.0, e_k_1 = 0.0;
  float Umax = 200, Umin = -200; /* Valores límite de la señal de control */
  float kp = -4.17540*5;
  float kd = -0.93737*5;

  /*Control*/
  ek = Rk - Yk; //Cálculo del error actual

  //Calculamos la señal de control incremental (PID)
  uk = kp * ek + kd * (ek - e_k_1) / Tm;

  e_k_1 = ek; //Actualizamos el error del instante anterior

  if (uk >= 5)
    Uk = Ueq + uk;
  else if (uk <= -5)
    Uk = -Ueq + uk;
  else
    Uk = 0;

  /* Saturación de la señal de control */
  if (Uk > Umax) Uks = Umax;
  else if (Uk < Umin) Uks = Umin;
  else Uks = Uk;

  return Uks;
}
