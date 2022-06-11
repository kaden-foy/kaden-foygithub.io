%% Clear workspace
clear all
close all
clc

%% inital guess
% lens 1
x1 = 0.025; % postion of first lens
r1 = 0.04; % radius of first lens
w1 = 0.0025;% width of first lens
% lens 2
x2 = 0.20; % postion of first lens
r2 = 0.12669; % radius of first lens
w2 = 0.015;% width of first lens

% lens 1
x1 = 0.025; % postion of first lens
r1 = 0.04; % radius of first lens
w1 = 0.0025;% width of first lens
% lens 2
x2 = 0.23; % postion of first lens
r2 = 0.1407; % radius of first lens
w2 = 0.02;% width of first lens


%% optimize parameters
% method 1
param = [x1,r1,w1,x2,r2,w2];
func = @(param) RAYTRACING(param(1),param(2),param(3),param(4),param(5),param(6));
f1 = func(param); % function before optimization
opt = fminsearch(func,param);
f2 = func(opt); % function after optimization

% method 2
% param = [x1,r1,w1,x2,r2,w2];
% func = @(param) RAYTRACING(param(1),param(2),param(3),param(4),param(5),param(6));
% A = [-1,0,0,0,0,0]; % x1 >= 0.01
% a = 0.01;
% B = [0,0,0,1,0,0]; % x2 <= 0.39
% b = 0.39;
% C = [1,0,0,-1,0,0]; % x1-x2 <= 0
% c = 0;
% f1 = func(param); % function before optimization
% opt = fmincon(func,param,A,a,B,b,C,c);
% f2 = func(opt); % function after optimization

% method 3
% f_div = @(r2) RAYTRACING_DIV(x1,r1,w1,x2,r2,w2);
% f_size = @(x2) RAYTRACING_SIZE(x1,r1,w1,x2,r2,w2);
% 
% xsearch = r2;
% divsearch = x2;
% for i = 1:10
%     xsearch = fminsearch(f_size,xsearch);
%     divsearch = fminsearch(f_div,divsearch);
% end
% r2 = xsearch;
% x2 = divsearch;
% opt = [x1,r1,w1,x2,r2,w2];

% method 4
% f_div = @(r2) RAYTRACING_DIV(x1,r1,w1,x2,r2,w2);
% f_size = @(x2) RAYTRACING_SIZE(x1,r1,w1,x2,r2,w2);
% 
% xsearch = r2;
% divsearch = x2;
% for i = 1:10
%     %xsearch = fminsearch(f_size,xsearch);
%     divsearch = fminsearch(f_div,divsearch);
% end
% r2 = xsearch;
% x2 = divsearch;
% opt = [x1,r1,w1,x2,r2,w2];

% print optimized parameters
%fprintf('OutOut of Function\n Before = %0.3d \n After = %0.3d \n',f1,f2)
fprintf('\nLens 1\n')
fprintf(' x = %0.2f [cm]\n r = %0.2f [cm]\n w = %0.2f [cm]\n',opt(1)*100,opt(2)*100,opt(3)*100)
fprintf('\nLens 2\n')
fprintf(' x = %0.2f [cm]\n r = %0.2f [cm]\n w = %0.2f [cm]\n',opt(4)*100,opt(5)*100,opt(6)*100)
%% Display Ray Tracing
% projector
pw = 0.0160; % projector width (apater)
ph = 0.0284; % projector hight
% lens 1
x1 = opt(1); % postion of first lens
r1 = opt(2); % radius of first lens
w1 = opt(3);% width of first lens
% lens 2
x2 = opt(4); % postion of first lens
r2 = opt(5); % radius of first lens
w2 = opt(6);% width of first lens
% container wall
x3 = 0.4; % postion of first lens
w3 = 0.005;% width of first lens
% beaker
Db_out = 0.07; % outer beaker diameter
Db_in = 0.067; % outer beaker diameter
h = 0.1; % high of beacker
xb = 0.5; % beaker location

nrays = 75; % number of rays

%% set up system
% create a container for optical elements (Bench class)
bench = Bench;

% aperture (simulate 
aper = Aperture( [ 0 0 -h/2 ], [ pw ph 1.5*ph 1.5*ph ] ); % rectangular aperture
bench.append( aper );

% Lens 1 pyrex, 
lens1_f = Lens( [ x1 0 -h/2 ], 1.25*ph, -r1, -1, { 'air' 'bk7' } ); % parabolic surface
lens1_b = Plane( [ x1+w1 0 -h/2 ], 1.3*ph, { 'bk7' 'air' } ); % parabolic surface
bench.append( {lens1_f lens1_b} );

% Lens 2
lens2_f = Plane( [ x2-w2 0 -h/2 ], 1.2*h, { 'air' 'bk7' } ); % parabolic surface
lens2_b = Lens( [ x2 0 -h/2 ], 1.2*h, -r2, -1, { 'bk7' 'air' } ); % parabolic surface
bench.append( {lens2_f lens2_b} );

% glass wall for container
wall1_f = Plane( [ x3-w3 0 -h/2 ], 1.25*h, { 'air' 'acrylic' } ); % parabolic surface
wall1_b = Plane( [ x3-w3 0 -h/2 ], 1.25*h, { 'acrylic' 'medium' } ); % parabolic surface
bench.append( {wall1_f wall1_b} );

% Beaker with resin
beak_f = CylinderLens( [ xb 0 0 ], Db_out, h, { 'medium' 'pyrex' } ); % beaker
beak_f.rotate([0,1,0],pi/2)
beak_b = CylinderLens( [ xb 0 0 ], Db_out, h, { 'pyrex' 'medium' } ); % beaker
beak_b.rotate([0,1,0],pi/2)

resin_f = CylinderLens( [ xb 0 0 ], Db_in, h, { 'pyrex' 'resin' } ); % resin
resin_f.rotate([0,1,0],pi/2)
resin_b = CylinderLens( [ xb 0 0 ], Db_in, h, { 'resin' 'pyrex' } ); % resin
resin_b.rotate([0,1,0],pi/2)

bench.append( {beak_f resin_f} );
bench.append( {resin_b beak_b} );

%% Ray tracing simulation
rays_in = Rays( nrays, 'collimated', [ -0.05 0 -h/2 ], [ 1 0 0 ], ph, 'square' ); 

% Draw layout
rays_through = bench.trace( rays_in );    % repeat to get the min spread rays
bench.draw( rays_through, 'lines' );  % display everything, the other draw option is 'lines'

%% Calculate dimensions and error
rays_in = Rays( 4000, 'collimated', [ -0.05 0 -h/2 ], [ 1 0 0 ], ph, 'square' ); 
rays = bench.trace( rays_in );    % repeat to get the min spread rays;
for i = 1 : length( rays ) - 1
    vis = ( rays( i ).I ~= 0 ) & ...
            isfinite( sum( rays( i ).r.^2, 2 ) ) & ...
            isfinite( sum( rays( i + 1 ).r.^2, 2 ) );  % visible rays
    real = dot( rays( i + 1 ).r - rays( i ).r, rays( i ).n, 2 ) > 0; % real rays (vs. virtual for virtual image)
    [ unique_colors, ~, ic ] = unique( rays( i ).color, 'rows' );
    for j = 1 : size( unique_colors, 1 )
        cvis = vis & real & ( ic == j );
        X = [ rays( i ).r( cvis, 1 )';  rays( i + 1 ).r( cvis, 1 )' ];
        Y = [ rays( i ).r( cvis, 2 )';  rays( i + 1 ).r( cvis, 2 )' ];
        Z = [ rays( i ).r( cvis, 3 )';  rays( i + 1 ).r( cvis, 3 )' ];
        if i == 10 % saving pints in beaker
            x = X;
            y = Y;
            z = Z;
        end
    end
end

% hieght of rays
h = max(z,[],2) - min(z,[],2);
h = mean(h); % - 0.096
% width of beam
w = max(y,[],2) - min(y,[],2);
w = mean(w); % - 0.054
% divergance
div_mag = sqrt(diff(y).^2 + diff(z).^2);
div = mean(div_mag); % - 0.0

fprintf('\nDimensions \n Width = %0.2f [cm] \n Hight = %0.2f [cm] \n Divergance = = %0.2f [um] \n ',w*100,h*100,div*1e6)

%% plot displacement plot
figure()
hold on
quiver(y(1,:),z(1,:),diff(y),diff(z))
% [yy,zz] = meshgrid(y(1,:),z(1,:));
% [xx,xx] = meshgrid(div_mag,div_mag);
% contour(yy,zz,xx)
hold off