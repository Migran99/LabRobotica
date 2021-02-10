% Función para mostrar telemetría de robot móvil
% Laboratorio de Robótica 4º GIERM 18-19
% Federico Cuesta
% USO: Pasar como parámetro el nombre del archivo de telemetría con el
% sigiente contenido por fila:
% -Incremento de tiempo transcurrido desde la lectura anterior (milisegundos),  
% -Distancia medida por sensor Izq/Frontal (cm), 
% -Distancia medida por sensor Dch/Trasero(cm), 
% -Referencia de control (cm), 
% -Modo activo (0: Parado, 1: Control frontal, … 4),
% -Velocidad PWM motor Izq (+/-255, negativo indica marcha atrás), 
% -Velocidad PWM motor Dch (+/-255, negativo indica marcha atrás).

function telemetria_odom(archivo)

%time,posx,posy,vx,vy,vL,vR,velIzq,velDer

tel=load(archivo);
muestras=length(tel);
disp('Incremento de tiempo mínimo:'); disp(min(tel(:,1)));
disp('Incremento de tiempo máximo:');disp(max(tel(:,1)));
disp('Incremento de tiempo promedio:'); disp(mean(tel(:,1)));
tiempo=zeros(1,muestras);
tiempo(1)=tel(1,1); %Vector de tiempo absoluto
for i=2:muestras
    tiempo(i)=tiempo(i-1)+tel(i,1);
end    

%Grafica 1
figure(1);
subplot(3,1,1:2)
plot(tel(:,2),tel(:,3))
xlabel('X (cm)');
ylabel('Y (cm)');
title('Trayectoria')
axis('equal')

subplot(3,1,3)
plot(tiempo,tel(:,2), tiempo,tel(:,3),tiempo,tel(:,10))
xlabel('Tiempo (ms)');
ylabel('X,Y (cm)');

%Grafica 2
figure(2)
subplot(3,1,1);
plot(tiempo,tel(:,4), tiempo,tel(:,5));
xlabel('Tiempo (ms)');
ylabel('Vx,Vy (cm/s)');

subplot(3,1,2);
plot(tiempo,tel(:,6), tiempo,tel(:,7));
xlabel('Tiempo (ms)');
ylabel('w,v ');

subplot(3,1,3);
plot(tiempo,tel(:,8), tiempo,tel(:,9));
xlabel('Tiempo (ms)');
ylabel('PWML,PWM');

end