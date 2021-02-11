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

%Calculo de puntos medidos
distPunto = tel(:,5);
phi = tel(:,4);
posx = tel(:,2);
posy = tel(:,3);

primVez = 1;
for i = 1:muestras
    
    if distPunto(i,1) > 0 && distPunto(i,1) < 50
        normPhi = phi(i,1)-pi/2;
        posx_obj = posx(i,1) + distPunto(i,1)*cos(normPhi);
        posy_obj = posy(i,1) + distPunto(i,1)*sin(normPhi);
        
        if primVez
            primVez = 0;
            posPunto = [posx_obj, posy_obj];
        else
            posPunto = [posPunto; posx_obj, posy_obj];
        end
    end
end


%Grafica 1
figure(1);
plot(tel(:,2),tel(:,3))
xlabel('X (cm)');
ylabel('Y (cm)');
title('Trayectoria')
axis('equal')

hold on;
plot(posPunto(:,1),posPunto(:,2),'*');

end