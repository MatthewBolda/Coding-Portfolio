using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class M4 : MonoBehaviour
{

    [SerializeField]
    private Camera cam;
    [SerializeField]
    private LayerMask mask;

    public Player player;

    ///////////////////////////////
    /// Differnt for each weapon //
    ///////////////////////////////
    public string name = "M4";

    public static int damage = 15;
    public float range = 100f;
    public float fireRate = 10f;

    /* stuff to add 
    public static float force = 10f;
    public static int max_ammo = 16;
    */

    ///////////////////////////////
    ///   Same for each weapon   //
    ///////////////////////////////
    ///
    void Start()
    {
        player = PlayerManager.instance.player1;
    }


    void Update()
    {
        if (Input.GetButtonDown("Fire1"))
        {
            Shoot();
        }
    }

    void Shoot()
    {
        RaycastHit _hit;
        if (Physics.Raycast(cam.transform.position, cam.transform.forward, out _hit, range, mask))
        {
            //debugger, we hit something
            Debug.Log("hit " + _hit.collider.name);
            ZombieController target = _hit.transform.GetComponent<ZombieController>();
            if (target != null)
            {
                if (target.currentHealth - damage <= 0)
                {
                    player.UpdatePoints(50);
                }
                else
                {
                    player.UpdatePoints(10);
                }
                target.TakeDamage(damage);
            }
        }
    }

}
