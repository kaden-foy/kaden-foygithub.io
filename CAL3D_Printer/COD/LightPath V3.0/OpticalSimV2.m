% clear workspace
%clear all
%close all
%clc

% System
% the system starts at x=0 and follows the x axis
% the normal surface is yz plane
bench = Bench;

%% Creating LED source
z = 5; % LED x location mm
y = 2.5; % LED y location mm
w = 1; % LED width mm
nrays = 1000;

rays_in = Rays( nrays, 'source', [ 0 3 3 ], [ 1 0 0 ], 0.75, 'square','air',405 );

%% Create Fresnal lens
x = 190; % distance from LED to lens mm
fprintf('length distance = %0.2f [mm]',x)
% % create flat plane
% lens1 = Plane( [x 0 0], 200, {'air' 'pmma'} );
% bench.append(lens1);

% This is the simulated aspheric lens surface
w = 2;
offset = 1.344;% This offset is because the lens
R = 100; % lens radius of curvature
k = -1; % lens aspheric constant
D = 230; % lens diameter
nrings = 60; % number of the Fresnel rings
x1 = 30;

% This simulates the above surface with a Fresnel surface
rads_outer = linspace( 5, D / 2, nrings ); % outer radii, the central ring's outer radius is 5 mm
rads_inner = [ 0 rads_outer( 1 : end - 1 ) ]; % the corresponding inner radii
d = sqrt( 1 - ( 1 + k ) * ( rads_inner / R ).^2 );
angs = pi/2 - atan( rads_inner ./ ( R * d ) ); % cone half-angle at the inner ring radius with respect to the x-axis direction
angs(1) = k; % replace the angle at the center (normally, pi/2) with the central part's conic constant k

% Fresnel surface made of quadric segments
pars = [ angs; ... 
         repmat( pi - pi/8, 1, length( angs ) ); ... % wall half-angle at the outer ring radius, here less vertical by pi/8 (optional, defaults to pi)
         R ./ rads_outer ]; % ring's radial radius of curvature in units of the ring's outer radius (optional, defaults to Inf, i.e. to a flat conical surface)
sags = zeros( length( rads_inner ), 1 ); % collapse the lens along its axis to a Fresnel structure
x1 = 42;
lens2 = FresnelLens( [ (x) 0 0 ], rads_outer, sags, pars, {'air'  'pmma' } ); % Fresnel surface piece-wise identical to the surface of the original lens
lens2.rotate( [ 0 0 1 ], 0 );
bench.append( lens2);

% create flat plane
lens1 = Plane( [(x+w) 0 0], 200, { 'pmma' 'air'} );
bench.append(lens1);

%Apperature (LCD screen)
d = 100; %distance between LCD and fresnel lens
aper = Aperture( [ (x+d) 0 0 ], [ 85 125 250 250 ] ); % circular aperture
bench.append( aper );

% Front wall
b = 0.01; % distance from LCD to beaker wall mm
w = 0.946; % wall thickness
lens31 = Plane( [(x+b+d) 0 0], 150, 150, { 'air' 'lcd'} );
bench.append(lens31);

lens41 = Plane( [(x+b+d+w) 0 0], 150, 150, { 'lcd' 'air'} );
bench.append(lens41);

% Front wall
b = 10; % distance from LCD to beaker wall mm
w = 3; % wall thickness
lens3 = Plane( [(x+b+d) 0 0], 150, 150, { 'air' 'pmma'} );
bench.append(lens3);

lens4 = Plane( [(x+b+d+w) 0 0], 150, 150, { 'pmma' 'medium'} );
bench.append(lens4);

% beaker
h = 130; % height of beaker
OD = 90; % outer diamter of beaker mm
t = 3; % thickness of beaker mm
p = 60; % centre of beaker from front wall mm
lens5 = CylinderLens( [ (x+b+d+p) 0 h/2 ], OD, h, { 'medium' 'pyrex' } ); % beaker
lens5.rotate([0 1 0],pi/2);
bench.append(lens5);
lens6 = CylinderLens( [ (x+b+d+p) 0 h/2 ], (OD-t), h, { 'pyrex' 'medium'} ); % beaker
lens6.rotate([0 1 0],pi/2);
bench.append(lens6);
lens7 = CylinderLens( [ (x+b+d+p) 0 h/2 ], (OD-t), h, { 'medium' 'pyrex' } ); % beaker
lens7.rotate([0 1 0],pi/2);
bench.append(lens7);
lens8 = CylinderLens( [ (x+b+d+p) 0 h/2 ], OD, h, { 'pyrex' 'medium' } ); % beaker
lens8.rotate([0 1 0],pi/2);
bench.append(lens8);

%% simulate ligth path
rays_through = bench.trace( rays_in );
bench.draw( rays_through, 'rays' );  % display everything, the other draw option is 'lines'

% percent of rays that make it through
perc = sum(rays_through(end).I~= 0)/sum(rays_through(1).I~= 0);
fprintf('\n percent of rays that go through beaker = %0.2f [ ] \n ',perc*100)
% plot light intsity
Int = [rays_through(:).I];
[w,c] = size(Int);
I = zeros(1,c);
for i = 1:c
    I(i) = mean(Int(Int(:,i)>0,i));
end

figure()
hold on
title('Intensity')
plot(I,'.r')
hold off

% plot points of intercetion
rays = rays_through;
r = OD/2;
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
div = sqrt(diff(y).^2 + diff(z).^2);
divMax = max(div);
div = mean(div); % - 0.0

fprintf('\nDimensions \n Width = %0.2f [mm] \n Hight = %0.2f [mm] \n Mean Divergance = %0.2f [mm] \n Max Divergance = %0.2f [mm] \n ',w,h,div,divMax)

figure()
hold on
quiver(y(1,:),z(1,:),diff(y),diff(z))
xlabel('x posistion [mm]')
ylabel('y posistion [mm]')
hold off
